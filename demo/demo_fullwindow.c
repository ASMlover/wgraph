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
#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../inc/common.h"
#include "../inc/wg_window.h"
#include "demo_tools.h"

#include "demo.h"



static const TCHAR g_message[]  = TEXT("Hello, world!");
static const TCHAR g_face[]     = TEXT("Times New Roman");
static const TCHAR g_hint[]     = TEXT("Press ESC to quit.");
static const TCHAR g_program[]  = TEXT("demo_fullwindow");


static void demo_keyDown(struct wgWindow* object, WPARAM wp, LPARAM lp)
{
  UNUSED_PARAM(lp)
  assert(NULL != object);

  if (VK_ESCAPE == wp)
    PostMessage(object->hWnd, WM_CLOSE, 0, 0);
}
static void demo_draw(struct wgWindow* object, HDC hDC)
{
  assert(NULL != object);

  TextOut(hDC, 0, 0, g_hint, _tcslen(g_hint));
  drawCenterText(hDC, GetDeviceCaps(hDC, HORZRES) / 2, 
      GetDeviceCaps(hDC, VERTRES) / 2, g_face, g_message, 72);
}


struct FullWindow {
  wgWindow_Inherit()
};

static struct wgWindow* fullWindowCreate(void)
{
  struct FullWindow* self;
  size_t size = sizeof(struct FullWindow);

  self = (struct FullWindow*)calloc(size, sizeof(char));
  if (NULL != self)
  {
    wgWindowInit((struct wgWindow*)self);
    self->draw    = demo_draw;
    self->keyDown = demo_keyDown;
  }

  return (struct wgWindow*)self;
}

static void fullWindowRelease(struct wgWindow** self)
{
  if (NULL != *self)
  {
    wgWindowUninit(*self);
    free(*self);
    *self = NULL;
  }
}


void demo_fullwindow(void* arg)
{
  HINSTANCE hInst = GetModuleHandle(NULL);
  struct wgWindow* fwin = fullWindowCreate();
  UNUSED_PARAM(arg)

  fprintf(stdout, "call function : %s, hInst = 0x%08X\n", __FUNCTION__, hInst);
  fwin->createEx(fwin, 0, g_program, g_program, WS_POPUP, 0, 0, 
      GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 
      NULL, NULL, hInst);
  wgWindowShow(fwin, SW_SHOWNORMAL);
  wgWindowUpdate(fwin);

  fwin->messageLoop(fwin);
  wgWindowRelease(&fwin);
}
