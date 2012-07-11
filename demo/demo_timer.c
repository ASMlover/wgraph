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
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../inc/common.h"
#include "../inc/wg_timer.h"

#include "demo.h"



void demo_timer(void* arg)
{
  struct wgTimer* timer = wgTimerCreate();
  char   buffer[128];
  unsigned int cpuSpeed10, useTime;

  UNUSED_PARAM(arg)
  fprintf(stdout, "call function : %s [demo of wgTimer]\n", __FUNCSIG__);

  wgTimerStart(timer);
  Sleep(1000);
  cpuSpeed10 = (unsigned int)(wgTimerStop(timer) / 100000);

  wgTimerStart(timer);
  CreateSolidBrush(RGB(0xAA, 0xAA, 0xAA));
  useTime = (unsigned int)wgTimerStop(timer);

  _stprintf(buffer, TEXT("CPU speed   %d.%d MHz\n")
      TEXT("wgTimer overhead %d clock cycles\n")
      TEXT("CreateSolidBrush %d clock cycles %d ns"), 
      cpuSpeed10 / 10, cpuSpeed10 % 10, (unsigned int)wgTimerOverhead(timer), 
      useTime, useTime * 10000 / cpuSpeed10);

  MessageBox(NULL, buffer, TEXT("How fast is GDI?"), MB_OK);

  wgTimerRelease(&timer);
}
