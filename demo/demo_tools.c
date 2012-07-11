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

#include "demo_tools.h"


#define TOOLS_DIVVALUE_DEF  (72)



void drawCenterText(HDC hDC, int x, int y, LPCTSTR face, LPCTSTR message, int point)
{
  HGDIOBJ hOld;
  HFONT   hFont = CreateFont(-point * GetDeviceCaps(hDC, LOGPIXELSY) / TOOLS_DIVVALUE_DEF, 
      0, 0, 0, FW_BOLD, TRUE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, 
      PROOF_QUALITY, VARIABLE_PITCH, face);
  assert(NULL != hFont);

  hOld = SelectObject(hDC, hFont);
  SetTextAlign(hDC, TA_CENTER | TA_BASELINE);

  SetBkMode(hDC, TRANSPARENT);
  SetTextColor(hDC, RGB(0, 0, 0xFF));
  TextOut(hDC, x, y, message, _tcslen(message));

  SelectObject(hDC, hOld);
  DeleteObject(hFont);
}
