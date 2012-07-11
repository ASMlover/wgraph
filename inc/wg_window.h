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
#ifndef __WG_WINDOW_HEADER_H__
#define __WG_WINDOW_HEADER_H__

struct wgWindow {
  HWND hWnd;
  void (*draw)(struct wgWindow*, HDC);
  void (*keyDown)(struct wgWindow*, WPARAM, LPARAM);
  LRESULT (*wndProc)(struct wgWindow*, HWND, UINT, WPARAM, LPARAM);
  void (*getWindowClassEx)(struct wgWindow*, WNDCLASSEX*);
  int (*createEx)(struct wgWindow*, DWORD, LPCTSTR, LPCTSTR, 
      DWORD, int, int, int, int, HWND, HMENU, HINSTANCE);
  WPARAM (*messageLoop)(struct wgWindow*);
};
#define wgWindow_Inherit()\
  HWND hWnd;\
  void (*draw)(struct wgWindow*, HDC);\
  void (*keyDown)(struct wgWindow*, WPARAM, LPARAM);\
  LRESULT (*wndProc)(struct wgWindow*, HWND, UINT, WPARAM, LPARAM);\
  void (*getWindowClassEx)(struct wgWindow*, WNDCLASSEX*);\
  int (*createEx)(struct wgWindow*, DWORD, LPCTSTR, LPCTSTR,\
      DWORD, int, int, int, int, HWND, HMENU, HINSTANCE);\
  WPARAM (*messageLoop)(struct wgWindow*);



extern struct wgWindow* wgWindowCreate(void);
extern void wgWindowRelease(struct wgWindow** object);
extern void wgWindowInit(struct wgWindow* object);
extern void wgWindowUninit(struct wgWindow* object);

extern HWND wgWindowHandle(struct wgWindow* object);
extern int wgWindowRegister(struct wgWindow* object, HINSTANCE hInst, LPCTSTR className);
extern int wgWindowShow(struct wgWindow* object, int cmdShow);
extern int wgWindowUpdate(struct wgWindow* object);

#endif  /* __WG_WINDOW_HEADER_H__ */
