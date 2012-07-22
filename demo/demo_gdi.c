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
#include <assert.h>
#include <stdlib.h>
#include "demo_gdi.h"


struct PenImpl {
  GDIObject_Inherit()
  LOGPEN logpen;
};


static int getObjectType(void* object)
{
  return OBJ_PEN;
}

static int unrealizeObject(void* object)
{
  return TRUE;
}

static int getObject(void* object, int bufferLen, void* buffer)
{
  struct PenImpl* self = (struct PenImpl*)object;

  assert(NULL != object);
  if (NULL == buffer)
    return sizeof(LOGPEN);
  else if (bufferLen >= sizeof(self->logpen))
  {
    memcpy(buffer, &self->logpen, sizeof(self->logpen));
    return sizeof(LOGPEN);
  }

  SetLastError(ERROR_INVALID_PARAMETER);
  return 0;
}

static int deleteObject(void* object)
{
  if (NULL != object)
  {
    free(object);
    return TRUE;
  }

  return FALSE;
}


extern struct Pen* penCreate(int style, int width, COLORREF color)
{
  struct PenImpl* object;
  size_t size = sizeof(struct PenImpl);

  object = (struct PenImpl*)calloc(size, sizeof(char));
  if (NULL != object)
  {
    object->logpen.lopnStyle    = style;
    object->logpen.lopnWidth.x  = width;
    object->logpen.lopnColor    = color;

    object->getObjectType   = getObjectType;
    object->getObject       = getObject;
    object->deleteObject    = deleteObject;
    object->unrealizeObject = unrealizeObject;
  }

  return (struct Pen*)object;
}
