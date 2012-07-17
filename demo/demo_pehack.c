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
#include "../inc/wg_pehack.h"

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


static void demo_pehack_setImport(HINSTANCE hInst)
{
  struct wgPEHack* pe = wgPEHackCreate(hInst);

  wgPEHackSetImportAddress(pe, "user32.dll", "MessageBoxA", (FARPROC)demo_displayMessageA);
  MessageBoxA(NULL, "Test", "wgPEHackSetImportAddress", MB_OK);
  
  wgPEHackRelease(&pe);
}

static void demo_pehack_setExport(void)
{
  HMODULE hUser = GetModuleHandle(TEXT("user32.dll"));
  struct wgPEHack* pe = wgPEHackCreate(hUser);
  FARPROC oldAddress, newAddress;
  char   buffer[128];

  oldAddress = (FARPROC)GetProcAddress(hUser, "MessageBoxA");
  wgPEHackSetExportAddress(pe, "MessageBoxA", (FARPROC)demo_displayMessageA);
  newAddress = (FARPROC)GetProcAddress(hUser, "MessageBoxA");

  sprintf(buffer, "GetProcAddress(MessageBoxA) changes from 0x%p to 0x%p", oldAddress, newAddress);
  MessageBoxA(NULL, buffer, "wgPEHackSetExportAddress", MB_OK);

  wgPEHackRelease(&pe);
}

static void demo_pehack_help(void)
{
  fprintf(stdout, 
    "  usage : wgraph [option1] [option2]\n\n"
    "  General option2:\n"
    "    -import    wgPEHack module SetImportAddress demo\n"
    "    -export    wgPEHack module SetExportAddress demo\n"
    );
}


void demo_pehack(void* arg)
{
  HINSTANCE hInst = GetModuleHandle(NULL);
  fprintf(stdout, "call function : %s, entry module is : 0x%8p\n", __FUNCTION__, hInst);

  if (NULL == arg)
    demo_pehack_help();
  else if (0 == stricmp("-import", (const char*)arg))
    demo_pehack_setImport(hInst);
  else if (0 == stricmp("-export", (const char*)arg))
  {
    demo_pehack_setImport(hInst);
    demo_pehack_setExport();        /* why set export address must be set import address first??? */
  }
  else
    demo_pehack_help();
}
