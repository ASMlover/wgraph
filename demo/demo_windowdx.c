/* 
 * Copyright (c) 2012, ASMlover. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list ofconditions and the following disclaimer.
 * 
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
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
/*
 * Copyright (c) 2012 ASMlove. All rights reserved.
 *
 *  http://blog.csdn.net/zfxfcx/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list ofconditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materialsprovided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY ASM "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ASMLOVE OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <ddraw.h>
#include <assert.h>
#include <stdio.h>
#include "../inc/common.h"
#include "../inc/wg_window.h"
#include "demo_tools.h"

#include "demo.h"

#pragma comment(lib, "ddraw.lib")
#pragma warning(disable:4201)


static const TCHAR* g_message = TEXT("Hello, world!");
static const TCHAR* g_face    = TEXT("Times New Roman");
static const TCHAR* g_hint    = TEXT("Press ESC to quit.");
static const TCHAR* g_program = TEXT("window_directX");


struct DXWindow {
  wgWindow_Inherit()
  LPDIRECTDRAW        ddraw;
  LPDIRECTDRAWSURFACE ddsurface;
};

static void __inline __blend(unsigned char* dest, unsigned char* src)
{
  dest[0] = (dest[0] + src[0]) / 2;
  dest[1] = (dest[1] + src[1]) / 2;
  dest[2] = (dest[2] + src[2]) / 2;
}

static void blend(struct DXWindow* self, int left, int top, int right, int bottom)
{
  DDSURFACEDESC   ddsd = {0};
  HRESULT         hr;
  unsigned char*  screen;
  int             x, y;

  assert(NULL != self);
  ddsd.dwSize = sizeof(DDSURFACEDESC);

  hr = IDirectDrawSurface_Lock(self->ddsurface, NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);
  assert(DD_OK == hr);
  screen = (unsigned char*)ddsd.lpSurface;

  for (y = top; y < bottom; ++y)
  {
    unsigned char* pixel = screen + y * ddsd.lPitch + left * 4;
    for (x = left; x < right; ++x, pixel += 4)
    {
      if (255 != pixel[0] || 255 != pixel[1] || 255 != pixel[2])
      {
        __blend(pixel - 4, pixel);
        __blend(pixel + 4, pixel);
        __blend(pixel - ddsd.lPitch, pixel);
        __blend(pixel + ddsd.lPitch, pixel);
      }
    }
  }

  IDirectDrawSurface_Unlock(self->ddsurface, ddsd.lpSurface);
}

static void draw(struct wgWindow* object, HDC hDC)
{
  assert(NULL != object && NULL != hDC);
  TextOut(hDC, 0, 0, g_hint, _tcslen(g_hint));
  drawCenterText(hDC, GetSystemMetrics(SM_CXSCREEN) / 2, 
    GetSystemMetrics(SM_CYSCREEN) / 2, g_face, g_message, 48);
  blend((struct DXWindow*)object, 80, 160, 560, 250);
}

static void keyDown(struct wgWindow* object, WPARAM wp, LPARAM lp)
{
  struct DXWindow* self = (struct DXWindow*)object;
  assert(NULL != self);
  UNUSED_PARAM(lp)

  if (VK_ESCAPE == wp)
    PostMessage(self->hWnd, WM_CLOSE, 0, 0);
}


static struct wgWindow* dxWindowCreate(void)
{
  struct DXWindow* object;
  size_t size = sizeof(struct DXWindow);

  object = (struct DXWindow*)calloc(size, sizeof(char));
  if (NULL != object)
  {
    wgWindowInit((struct wgWindow*)object);
    object->draw    = draw;
    object->keyDown = keyDown;
  }
  return (struct wgWindow*)object;
}

static void dxWindowRelase(struct wgWindow** object)
{
  struct DXWindow* self = (struct DXWindow*)*object;

  if (NULL != *object)
  {
    if (NULL != self->ddsurface)
      IDirectDrawSurface_Release(self->ddsurface);
    if (NULL != self->ddraw)
      IDirectDraw_Release(self->ddraw);

    wgWindowUninit(*object);
    free(*object);
    *object = NULL;
  }
}

static int dxWindowCreateSurface(struct DXWindow* self)
{
  HRESULT hr;
  DDSURFACEDESC ddsd = {0};

  assert(NULL != self);
  hr = DirectDrawCreate(NULL, &self->ddraw, NULL);
  if (DD_OK != hr)
    return Result_Failed;

  hr = IDirectDraw_SetCooperativeLevel(self->ddraw, self->hWnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE);
  if (DD_OK != hr)
    return Result_Failed;

  hr = IDirectDraw_SetDisplayMode(self->ddraw, 640, 480, 32);
  if (DD_OK != hr)
    return Result_Failed;

  ddsd.dwSize   = sizeof(ddsd);
  ddsd.dwFlags  = DDSD_CAPS;
  ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

  return (DD_OK == IDirectDraw_CreateSurface(self->ddraw, &ddsd, &self->ddsurface, NULL) 
      ? Result_Success : Result_Failed);
}

void demo_windowdx(void* arg)
{
  struct wgWindow* dxwin = dxWindowCreate();
  HINSTANCE hInst = GetModuleHandle(NULL);

  UNUSED_PARAM(arg)
  fprintf(stdout, "call function : %s\n", __FUNCTION__);

  dxwin->createEx(dxwin, 0, g_program, g_program, 
      WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN), 
      GetSystemMetrics(SM_CYSCREEN), NULL, NULL, hInst);
  dxWindowCreateSurface((struct DXWindow*)dxwin);
  wgWindowShow(dxwin, SW_SHOWNORMAL);
  wgWindowUpdate(dxwin);

  dxwin->messageLoop(dxwin);

  dxWindowRelase(&dxwin);
}
