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
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include "../inc/pefile.h"

#include "demo.h"



static int WINAPI demo_displayMessageA(HWND hWnd, LPCSTR text, LPCSTR caption, UINT type)
{
  WCHAR textW[MAX_PATH] = {0};
  WCHAR captionW[MAX_PATH] = {0};

  MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, text, -1, textW, MAX_PATH);
  wcscat(textW, L"-intercepted");

  MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, caption, -1, captionW, MAX_PATH);
  wcscat(captionW, L"-intercepted");

  return MessageBoxW(hWnd, textW, captionW, type);
}


void demo_pefile(void* arg)
{
  HINSTANCE hInst = GetModuleHandle(NULL);
  struct PEFile* pe = peFileCreate(hInst);
  fprintf(stdout, "call function : %s, entry module is : 0x%8p\n", __FUNCTION__, hInst);

  peFileSetImportAddress(pe, "user32.dll", "MessageBoxA", (FARPROC)demo_displayMessageA);

  MessageBoxA(NULL, "Test", "peFileSetImportAddress", MB_OK);

  peFileRelease(&pe);
}
