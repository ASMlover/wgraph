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
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../inc/common.h"

#include "../inc/wg_window.h"



static LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

/*!< VIRTUAL INTERFACES */
static void drawDef(struct wgWindow* object, HDC hDC)
{
  UNUSED_PARAM(object)
  UNUSED_PARAM(hDC)
}
static void keyDownDef(struct wgWindow* object, WPARAM wp, LPARAM lp)
{
  UNUSED_PARAM(object)
  UNUSED_PARAM(wp)
  UNUSED_PARAM(lp)
}
static LRESULT wndProcDef(struct wgWindow* object, HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
  assert(NULL != object);
  switch (msg)
  {
  case WM_KEYDOWN:
    object->keyDown(object, wp, lp); 
    return 0;
  case WM_PAINT:
    {
      PAINTSTRUCT ps;

      BeginPaint(hWnd, &ps);
      object->draw(object, ps.hdc);
      EndPaint(hWnd, &ps);
    } 
    return 0;
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }

  return DefWindowProc(hWnd, msg, wp, lp);
}
static void getWindowClassExDef(struct wgWindow* object, WNDCLASSEX* wc)
{
  assert(NULL != object && NULL != wc);
  memset(wc, 0, sizeof(WNDCLASSEX));

  wc->cbSize        = sizeof(WNDCLASSEX);
  wc->lpfnWndProc   = (WNDPROC)wndProc;
  wc->hCursor       = LoadCursor(NULL, IDC_ARROW);
  wc->hIcon         = LoadIcon(NULL, IDI_APPLICATION);
  wc->hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
}
static int createExDef(struct wgWindow* object, DWORD styleEx, LPCTSTR className, LPCTSTR windowName, 
  DWORD style, int x, int y, int width, int height, HWND hParent, HMENU hMenu, HINSTANCE hInst)
{
  MDICREATESTRUCT  mdic = {0};

  assert(NULL != object);
  if (Result_Success != wgWindowRegister(object, hInst, className))
    return Result_Failed;
  mdic.lParam = (LPARAM)object;
  object->hWnd = CreateWindowEx(styleEx, className, windowName, 
    style, x, y, width, height, hParent, hMenu, hInst, &mdic);

  return (NULL != object->hWnd ? Result_Success : Result_Failed);
}
static WPARAM messageLoopDef(struct wgWindow* object)
{
  MSG msg;
  assert(NULL != object);

  while (GetMessage(&msg, NULL, 0, 0) > 0)
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}

static LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
  struct wgWindow* object;

  if (WM_NCCREATE == msg)
  {
    MDICREATESTRUCT* mdic;
    
    assert(!IsBadReadPtr((void*)lp, sizeof(CREATESTRUCT)));
    mdic = (MDICREATESTRUCT*)((LPCREATESTRUCT)lp)->lpCreateParams;
    object = (struct wgWindow*)(mdic->lParam);

    assert(!IsBadReadPtr(object, sizeof(struct wgWindow)));
    SetWindowLong(hWnd, GWL_USERDATA, (LONG)object);
  }
  else 
    object = (struct wgWindow*)GetWindowLong(hWnd, GWL_USERDATA);

  if (NULL != object)
    return wndProcDef(object, hWnd, msg, wp, lp);
  else 
    return DefWindowProc(hWnd, msg, wp, lp);
}



/*!< OBJECT OPERATION INTERFACES */
struct wgWindow* wgWindowCreate(void)
{
  struct wgWindow* object;
  size_t size = sizeof(struct wgWindow);

  object = (struct wgWindow*)calloc(size, sizeof(char));
  if (NULL != object)
    wgWindowInit(object);

  return object;
}

void wgWindowRelease(struct wgWindow** object)
{
  if (NULL != *object)
  {
    wgWindowUninit(*object);
    free(*object);
    *object = NULL;
  }
}

void wgWindowInit(struct wgWindow* object)
{
  if (NULL != object)
  {
    object->draw             = drawDef;
    object->keyDown          = keyDownDef;
    object->wndProc          = wndProcDef;
    object->getWindowClassEx = getWindowClassExDef;
    object->createEx         = createExDef;
    object->messageLoop      = messageLoopDef;
  }
}

void wgWindowUninit(struct wgWindow* object)
{
  UNUSED_PARAM(object)
}

HWND wgWindowHandle(struct wgWindow* object)
{
  assert(NULL != object);
  return object->hWnd;
}

int wgWindowRegister(struct wgWindow* object, HINSTANCE hInst, LPCTSTR className)
{
  WNDCLASSEX wc;

  assert(NULL != object);
  if (!GetClassInfoEx(hInst, className, &wc))
  {
    object->getWindowClassEx(object, &wc);
    wc.hInstance      = hInst;
    wc.lpszClassName  = className;
    if (!RegisterClassEx(&wc))
      return Result_Failed_RegisterWindowClass;
  }

  return Result_Success;
}

int wgWindowShow(struct wgWindow* object, int cmdShow)
{
  assert(NULL != object);

  if (ShowWindow(object->hWnd, cmdShow))
    return Result_Success;
  return Result_Failed;
}

int wgWindowUpdate(struct wgWindow* object)
{
  assert(NULL != object);

  if (UpdateWindow(object->hWnd))
    return Result_Success;
  return Result_Failed;
}
