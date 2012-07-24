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
#include <string.h>

#include "demo_gditable.h"

struct GdiTable {
  struct GdiTableCell* tables;
};

typedef UINT (CALLBACK *pPROC)(void);
struct GdiTable* gdiTableCreate(void)
{
  struct GdiTable* object;
  size_t size = sizeof(struct GdiTable);

  object = (struct GdiTable*)calloc(size, sizeof(char));
  if (NULL != object)
  {
    pPROC p = (pPROC)GetProcAddress(GetModuleHandle(TEXT("GDI32.DLL")), "GdiQueryTable");
    assert(NULL != p);

    if (NULL != p)
      object->tables = (struct GdiTableCell*)p();

    if (NULL == object->tables) 
    {
      fprintf(stdout, "%s (%d) : error ...\n", __FILE__, __LINE__);
      exit(1);
    }
  }
  return object;
}

void gdiTableRelease(struct GdiTable** object)
{
  if (NULL != *object)
  {
    free(*object);
    *object = NULL;
  }
}

struct GdiTableCell gdiTableGetByHandle(struct GdiTable* object, HGDIOBJ handle)
{
  assert(NULL != object);
  return object->tables[(UINT)handle & 0xFFFF];
}

struct GdiTableCell gdiTableGetByIndex(struct GdiTable* object, unsigned int index)
{
  assert(NULL != object);
  return object->tables[index & 0xFFFF];
}
