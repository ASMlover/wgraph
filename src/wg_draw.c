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



void* wgDrawCreate(HWND main_hwnd, HINSTANCE main_instance)
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

void wgDrawRelease(void** object)
{
  if (NULL != *object)
  {
    wgDrawUninit(*object);
    free(*object);
    *object = NULL;
  }
}

int wgDrawInit(void* object, int width, int height, int bpp)
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
  for (i = 0; i < WG_PALETTES_DEF; ++i)
  {
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

void wgDrawUninit(void* object)
{
}

LPDIRECTDRAWCLIPPER wgDrawAttachClipper(void* object, LPDIRECTDRAWSURFACE7 lpdds, int numRects, LPRECT clipList)
{
  return NULL;
}

int wgDrawFlip(void* object)
{
  return Result_Success;
}

int wgDrawFillSurface(void* object, LPDIRECTDRAWSURFACE7 lpdds, int color)
{
  return Result_Success;
}

int wgDrawRectangle(void* object, int left, int top, int right, int bottom, int color)
{
  return Result_Success;
}

int wgDrawTextGdi(void* object, int x, int y, LPCTSTR text, int color)
{
  return Result_Success;
}





void* wgClockCreate(void)
{
  return NULL;
}

void wgClockRelease(void** object)
{
}

DWORD wgClockStart(void* object)
{
  return 0;
}

DWORD wgClockGet(void* object)
{
  return 0;
}

DWORD wgClockWait(void* object, DWORD count)
{
  return 0;
}
