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
#ifndef __WG_DRAW_HEADER_H__
#define __WG_DRAW_HEADER_H__

#define WG_SCREEN_WIDTH         (640)
#define WG_SCREEN_HEIGHT        (480)
#define WG_SCREEN_BPP           (8)     /**!< bits per pixel */
#define WG_MAX_COLORS           (256)   /**!< maximum colors */

#define WG_KEYDOWN(vk_code)     ((GetAsyncKeyState((vk_code)) & 0x8000) ? 1 : 0)
#define WG_KEYUP(vk_code)       ((GetAsyncKeyState((vk_code)) & 0x8000) ? 0 : 1)

/**! initializes a direct draw struct object */
#define WG_DDRAWSTRUCT_INIT(s)  { memset(&(s), 0, sizeof(s)); (s).dwSize = sizeof(s); }

/**! direct draw functions */
extern void* wgDrawCreate(HWND main_hwnd, HINSTANCE main_instance);
extern void wgDrawRelease(void** object);

extern int wgDrawInit(void* object, int width, int height, int bpp);
extern void wgDrawUninit(void* object);
extern LPDIRECTDRAWCLIPPER wgDrawAttachClipper(void* object, LPDIRECTDRAWSURFACE7 lpdds, int numRects, LPRECT clipList);
extern int wgDrawFlip(void* object);
extern int wgDrawFillSurface(void* object, LPDIRECTDRAWSURFACE7 lpdds, int color);
extern int wgDrawRectangle(void* object, int left, int top, int right, int bottom, int color);
extern int wgDrawTextGdi(void* object, int x, int y, LPCTSTR text, int color);


extern void* wgClockCreate(void);
extern void wgClockRelease(void** object);
extern DWORD wgClockStart(void* object);
extern DWORD wgClockGet(void* object);
extern DWORD wgClockWait(void* object, DWORD count);

#endif  /* __WG_DRAW_HEADER_H__ */
