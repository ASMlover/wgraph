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
#include <stdio.h>
#include "../inc/common.h"
#include "demo.h"
#include "demo_gdi.h"


void demo_gdi_test(void* arg)
{
  struct Pen* pen = penCreate(PS_SOLID, 1, RGB(0, 0, 0xFF));

  UNUSED_PARAM(arg)
  fprintf(stdout, "call function : %s\n", __FUNCTION__);

  if (NULL != pen)
    fprintf(stdout, "\tcreate pen object success ...\n");
  else 
    fprintf(stdout, "\tcreate pen object failed ...\n");

  fprintf(stdout, "\tget pen object type is : %d\n", pen->getObjectType(pen));
  {
    LOGPEN lp;
    pen->getObject(pen, sizeof(lp), &lp);
    fprintf(stdout, "\tget object {lopnStyle=>%d,lopnWidth{x=>%d,y=>%d},lopnColor=>%u}\n", 
        lp.lopnStyle, lp.lopnWidth.x, lp.lopnWidth.y, lp.lopnColor);
  }

  if (NULL != pen)
  {
    pen->deleteObject(pen);
    pen = NULL;
  }
}
