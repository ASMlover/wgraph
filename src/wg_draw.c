/*
 * Copyright (c) 2012 ASMlover. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list ofconditions and the following disclaimer.
 *
 *    notice, this list of conditions and the following disclaimer in
 *  * Redistributions in binary form must reproduce the above copyright
 *    the documentation and/or other materialsprovided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#define STRICT
#define WIN32_LEAN_AND_MEAN
#define INITGUID
#include <windows.h>
#include <tchar.h>
#include <ddraw.h>
#include "../inc/common.h"

#include "../inc/wg_draw.h"

#pragma comment(lib, "ddraw.lib")



#define WG_PALETTES_DEF  (256)

struct wgDraw {
  HWND        main_hwnd;
  HINSTANCE   main_instance;

  LPDIRECTDRAW7         lpdd;
  LPDIRECTDRAWSURFACE7  lpddsprimary;
  LPDIRECTDRAWSURFACE7  lpddsback;
  LPDIRECTDRAWPALETTE   lpddpal;
  LPDIRECTDRAWCLIPPER   lpddclipper;
  PALETTEENTRY          palette[WG_PALETTES_DEF];
  DDSURFACEDESC2        ddsd;
  DDSCAPS2              ddscaps;

  int minClipX;
  int minClipY;
  int maxClipX;
  int maxClipY;
  int screenWidth;
  int screenHeight;
  int screenBpp;
};

struct wgClock {
  DWORD startCount;
};



void* 
wgDrawCreate(HWND main_hwnd, HINSTANCE main_instance)
{
  struct wgDraw* object = (struct wgDraw*)calloc(1, sizeof(*object));

  if (NULL != object) {
    object->main_hwnd     = main_hwnd;
    object->main_instance = main_instance;
    object->maxClipX      = WG_SCREEN_WIDTH - 1;
    object->maxClipY      = WG_SCREEN_HEIGHT - 1;
    object->screenWidth   = WG_SCREEN_WIDTH;
    object->screenHeight  = WG_SCREEN_HEIGHT;
    object->screenBpp     = WG_SCREEN_BPP;
  }
  return object;
}

void 
wgDrawRelease(void** object)
{
  if (NULL != *object)
  {
    wgDrawUninit(*object);
    free(*object);
    *object = NULL;
  }
}

int 
wgDrawInit(void* object, int width, int height, int bpp)
{
  int i;
  RECT screenRect;
  struct wgDraw* self = (struct wgDraw*)object;

  if (NULL == self)
    return Result_Failed_InvalidParameters;

  if (DD_OK != DirectDrawCreateEx(NULL, (void**)&self->lpdd, &IID_IDirectDraw7, NULL))
    return Result_Failed;
  if (DD_OK != IDirectDraw7_SetCooperativeLevel(self->lpdd, 
    self->main_hwnd, DDSCL_ALLOWMODEX | DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT))
    return Result_Failed;
  if (DD_OK != IDirectDraw7_SetDisplayMode(self->lpdd, width, height, bpp, 0, 0))
    return Result_Failed;

  self->screenWidth   = width;
  self->screenHeight  = height;
  self->screenBpp     = bpp;

  self->ddsd.dwSize   = sizeof(self->ddsd);
  self->ddsd.dwFlags  = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
  self->ddsd.ddsCaps.dwCaps     = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
  self->ddsd.dwBackBufferCount  = 1;

  IDirectDraw7_CreateSurface(self->lpdd, &self->ddsd, &self->lpddsprimary, NULL);
  self->ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
  IDirectDrawSurface7_GetAttachedSurface(self->lpddsprimary, &self->ddscaps, &self->lpddsback);

  memset(self->palette, 0, sizeof(self->palette));
  for (i = 0; i < WG_PALETTES_DEF; ++i) {
    if (i < 64)
      self->palette[i].peRed = 4 * i;
    else if (i >= 64 && i < 128)
      self->palette[i].peGreen = (i - 64) * 4;
    else if (i >= 128 && i < 192)
      self->palette[i].peBlue = (i - 128) * 4;
    else if (i >= 192 && i < 256)
      self->palette[i].peRed = self->palette[i].peGreen = self->palette[i].peBlue = (i - 192) * 4;
    self->palette[i].peFlags = PC_NOCOLLAPSE;
  }

  if (DD_OK != IDirectDraw7_CreatePalette(self->lpdd, 
    DDPCAPS_8BIT | DDPCAPS_INITIALIZE | DDPCAPS_ALLOW256, self->palette, &self->lpddpal, NULL))
    return Result_Failed;
  if (DD_OK != IDirectDrawSurface7_SetPalette(self->lpddsprimary, self->lpddpal))
    return Result_Failed;

  wgDrawFillSurface(self, self->lpddsprimary, 0);
  wgDrawFillSurface(self, self->lpddsback, 0);

  screenRect.left   = 0;
  screenRect.top    = 0;
  screenRect.right  = self->screenWidth;
  screenRect.bottom = self->screenHeight;
  self->lpddclipper = wgDrawAttachClipper(self, self->lpddsback, 1, &screenRect);

  return Result_Success;
}

void 
wgDrawUninit(void* object)
{
  struct wgDraw* self = (struct wgDraw*)object;
  if (NULL == self)
    return;

  if (NULL != self->lpddclipper) {
    IDirectDrawClipper_Release(self->lpddclipper);
    self->lpddclipper = NULL;
  }
  if (NULL != self->lpddpal) {
    IDirectDrawPalette_Release(self->lpddpal);
    self->lpddpal = NULL;
  }
  if (NULL != self->lpddsback) {
    IDirectDrawSurface7_Release(self->lpddsback);
    self->lpddsback = NULL;
  }
  if (NULL != self->lpddsprimary) {
    IDirectDrawSurface7_Release(self->lpddsprimary);
    self->lpddsprimary = NULL;
  }
  if (NULL != self->lpdd) {
    IDirectDraw7_Release(self->lpdd);
    self->lpdd = NULL;
  }
}

LPDIRECTDRAWCLIPPER 
wgDrawAttachClipper(void* object, LPDIRECTDRAWSURFACE7 lpdds, int numRects, LPRECT clipList)
{
  int i;
  LPRGNDATA region_data;
  LPDIRECTDRAWCLIPPER lpddclipper;

  struct wgDraw* self = (struct wgDraw*)object;
  if (NULL == self)
    return NULL;

  if (DD_OK != IDirectDraw7_CreateClipper(self->lpdd, 0, &lpddclipper, NULL))
    return NULL;
  region_data = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER) + numRects * sizeof(RECT));
  memcpy(region_data->Buffer, clipList, numRects * sizeof(RECT));
  region_data->rdh.dwSize   = sizeof(RGNDATAHEADER);
  region_data->rdh.iType    = RDH_RECTANGLES;
  region_data->rdh.nCount   = numRects;
  region_data->rdh.nRgnSize = numRects * sizeof(RECT);
  region_data->rdh.rcBound.left   = 64000;
  region_data->rdh.rcBound.top    = 64000;
  region_data->rdh.rcBound.right  = -64000;
  region_data->rdh.rcBound.bottom = -64000;

  for (i = 0; i < numRects; ++i) {
    if (clipList[i].left < region_data->rdh.rcBound.left)
      region_data->rdh.rcBound.left = clipList[i].left;
    if (clipList[i].top < region_data->rdh.rcBound.top)
      region_data->rdh.rcBound.top = clipList[i].top;
    if (clipList[i].right > region_data->rdh.rcBound.right)
      region_data->rdh.rcBound.right = clipList[i].right;
    if (clipList[i].bottom > region_data->rdh.rcBound.bottom)
      region_data->rdh.rcBound.bottom = clipList[i].bottom;
  }

  if (DD_OK != IDirectDrawClipper_SetClipList(lpddclipper, region_data, 0)) {
    free(region_data);
    return NULL;
  }
  if (DD_OK != IDirectDrawSurface7_SetClipper(lpdds, lpddclipper)) {
    free(region_data);
    return NULL;
  }

  free(region_data);
  return lpddclipper;
}

int 
wgDrawFlip(void* object)
{
  struct wgDraw* self = (struct wgDraw*)object;

  if (NULL == self)
    return Result_Failed;
  while (DD_OK != IDirectDrawSurface_Flip(self->lpddsprimary, NULL, DDFLIP_WAIT)) {
  }

  return Result_Success;
}

int 
wgDrawFillSurface(void* object, LPDIRECTDRAWSURFACE7 lpdds, int color)
{
  DDBLTFX ddbltfx;
  struct wgDraw* self = (struct wgDraw*)object;

  if (NULL == self)
    return Result_Failed;

  WG_DDRAWSTRUCT_INIT(ddbltfx);
  ddbltfx.dwFillColor = color;

  IDirectDrawSurface7_Blt(lpdds, NULL, NULL, NULL, 
    DDBLT_COLORFILL | DDBLT_WAIT | DDBLT_ASYNC, &ddbltfx);

  return Result_Success;
}

int 
wgDrawRectangle(void* object, int left, int top, int right, int bottom, int color)
{
  DDBLTFX ddbltfx;
  RECT fill_area = {left, top, right + 1, bottom + 1};
  struct wgDraw* self = (struct wgDraw*)object;

  if (NULL == self)
    return Result_Failed;
  WG_DDRAWSTRUCT_INIT(ddbltfx);
  ddbltfx.dwFillColor = color;

  IDirectDrawSurface7_Blt(self->lpddsback, &fill_area, NULL, NULL, 
    DDBLT_COLORFILL | DDBLT_WAIT | DDBLT_ASYNC, &ddbltfx);

  return Result_Success;
}

int 
wgDrawTextGdi(void* object, int x, int y, LPCTSTR text, int color)
{
  HDC hDC;
  struct wgDraw* self = (struct wgDraw*)object;

  if (NULL == self)
    return Result_Failed;

  if (DD_OK != IDirectDrawSurface7_GetDC(self->lpddsback, &hDC))
    return Result_Failed;
  SetTextColor(hDC, RGB(self->palette[color].peRed, self->palette[color].peGreen, self->palette[color].peBlue));
  SetBkMode(hDC, TRANSPARENT);
  TextOut(hDC, x, y, text, _tcslen(text));

  IDirectDrawSurface7_ReleaseDC(self->lpddsback, hDC);
  return Result_Success;
}





void* 
wgClockCreate(void)
{
  struct wgClock* object = (struct wgClock*)calloc(1, sizeof(*object));
  return object;
}

void 
wgClockRelease(void** object)
{
  if (NULL != *object) {
    free(*object);
    *object = NULL;
  }
}

DWORD 
wgClockStart(void* object)
{
  struct wgClock* self = (struct wgClock*)object;
  
  if (NULL != self)
    return (self->startCount = wgClockGet(self));
  return 0UL;
}

DWORD 
wgClockGet(void* object)
{
  return (NULL != object ? GetTickCount() : 0UL);
}

DWORD wgClockWait(void* object, DWORD count)
{
  struct wgClock* self = (struct wgClock*)object;

  if (NULL != self) {
    while ((wgClockGet(self) - self->startCount) < count) {
    }

    return wgClockGet(self);
  }
  return 0UL;
}
