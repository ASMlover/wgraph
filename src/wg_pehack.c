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
#include <stdio.h>
#include <stdlib.h>
#include "../inc/common.h"

#include "../inc/wg_pehack.h"


static BOOL WINAPI 
HackWriteProcessMemory(HANDLE hProcess, void* dest, void* source, DWORD sourceLen, DWORD* outWriteLen)
{
  __try 
  {
    memcpy(dest, source, sourceLen);
  }
  __except (EXCEPTION_EXECUTE_HANDLER)
  {
    __try 
    {
      MEMORY_BASIC_INFORMATION mbi;
      VirtualQuery(dest, &mbi, sizeof(mbi));
      VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_READWRITE, &mbi.Protect);
      memcpy(dest, source, sourceLen);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
      return FALSE;
    }
  }

  *outWriteLen = sourceLen;
  return TRUE;
}



struct wgPEHack {
  const char*         moduleHandler;
  PIMAGE_DOS_HEADER   dosHeader;
  PIMAGE_NT_HEADERS   ntHeader;
};

static const char* rva2ptr(struct wgPEHack* object, unsigned int rva)
{
  if ((NULL != object->moduleHandler) && (0u != rva))
    return (object->moduleHandler + rva);
  else
    return NULL;
}

static void peFileInit(struct wgPEHack* object, HMODULE moduleHandler)
{
  object->moduleHandler = (const char*)moduleHandler;
  if (IsBadReadPtr(moduleHandler, sizeof(IMAGE_DOS_HEADER)))
  {
    object->dosHeader = NULL;
    object->ntHeader  = NULL;
  }
  else 
  {
    object->dosHeader = (PIMAGE_DOS_HEADER)object->moduleHandler;
    if (IsBadReadPtr(rva2ptr(object, object->dosHeader->e_lfanew), sizeof(IMAGE_NT_HEADERS)))
      object->ntHeader = NULL;
    else
      object->ntHeader = (PIMAGE_NT_HEADERS)rva2ptr(object, object->dosHeader->e_lfanew);
  }
}

struct wgPEHack* wgPEHackCreate(HMODULE moduleHandler)
{
  struct wgPEHack* object;
  size_t size = sizeof(struct wgPEHack);

  object = (struct wgPEHack*)calloc(size, sizeof(char));
  if (NULL != object)
    peFileInit(object, moduleHandler);

  return object;
}

void wgPEHackRelease(struct wgPEHack** object)
{
  if (NULL != *object)
  {
    free(*object);
    *object = NULL;
  }
}

const void* wgPEHackGetDirectory(struct wgPEHack* object, int id)
{
  assert(NULL != object);
  return rva2ptr(object, object->ntHeader->OptionalHeader.DataDirectory[id].VirtualAddress);
}

PIMAGE_IMPORT_DESCRIPTOR wgPEHackGetImportDescriptor(struct wgPEHack* object, LPCSTR dllName)
{
  if (NULL != object && NULL != dllName)
  {
    PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)wgPEHackGetDirectory(object, IMAGE_DIRECTORY_ENTRY_IMPORT);

    if (NULL == pImport)
      return NULL;
    while (pImport->FirstThunk)
    {
      if (0 == stricmp(dllName, rva2ptr(object, pImport->Name)))
        return pImport;
      ++pImport;
    }
  }
  return NULL;
}

const UINT* wgPEHackGetFunctionPtr(struct wgPEHack* object, PIMAGE_IMPORT_DESCRIPTOR import, LPCSTR functionName)
{
  if (NULL != object && NULL != import && NULL != functionName)
  {
    int  i;
    BOOL matched;
    PIMAGE_THUNK_DATA thunk = (PIMAGE_THUNK_DATA)rva2ptr(object, import->OriginalFirstThunk);

    for (i = 0; thunk->u1.Function; ++i)
    {
      if (0x80000000 & thunk->u1.Ordinal)
        matched = ((thunk->u1.Ordinal & 0xFFFF) == (DWORD)functionName);
      else 
        matched = (0 == stricmp(functionName, rva2ptr(object, (unsigned int)thunk->u1.AddressOfData) + 2));

      if (matched)
        return (unsigned int*)rva2ptr(object, import->FirstThunk) + i;
      ++thunk;
    }
  }
  return NULL;
}

FARPROC wgPEHackSetImportAddress(struct wgPEHack* object, LPCSTR dllName, LPCSTR functionName, FARPROC functionAddress)
{
  if (NULL != object && NULL != dllName && NULL != functionName && NULL != functionAddress)
  {
    PIMAGE_IMPORT_DESCRIPTOR import = wgPEHackGetImportDescriptor(object, dllName);

    if (NULL != import)
    {
      FARPROC oldFuncAddress;
      DWORD   writeLen;
      const unsigned int* functionPtr = wgPEHackGetFunctionPtr(object, import, functionName);
      if (IsBadReadPtr(functionPtr, sizeof(unsigned int)))
        return NULL;

      oldFuncAddress = (FARPROC)*functionPtr;
      HackWriteProcessMemory(GetCurrentProcess(), (void*)functionPtr, &functionAddress, sizeof(DWORD), &writeLen);
      return oldFuncAddress;
    }
  }
  return NULL;
}

FARPROC wgPEHackSetExportAddress(struct wgPEHack* object, LPCSTR functionName, FARPROC functionAddress)
{
  if (NULL != object && NULL != functionName && NULL != functionAddress)
  {
    unsigned int ordinal = 0;
    PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)wgPEHackGetDirectory(object, IMAGE_DIRECTORY_ENTRY_EXPORT);

    DWORD* rvaPtr;
    DWORD  result, writeLen = 0, newRvaValue;

    if (NULL == pExport)
      return NULL;
    if ((unsigned int)functionName < 0xFFFF)
      ordinal = (unsigned int)functionName;
    else
    {
      const unsigned long* namesArray = (const unsigned long*)rva2ptr(object, pExport->AddressOfNames);
      const unsigned short* ordinalsArray = (const unsigned short*)rva2ptr(object, pExport->AddressOfNameOrdinals);
      unsigned int i;
      for (i = 0u; i < pExport->AddressOfNames; ++i)
      {
        if (0 == stricmp(functionName, rva2ptr(object, namesArray[i])))
        {
          ordinal = pExport->Base + ordinalsArray[i];
          break;
        }
      }
    }

    if ((ordinal < pExport->Base) || (ordinal > pExport->NumberOfFunctions))
      return NULL;

    rvaPtr = (DWORD*)rva2ptr(object, pExport->AddressOfFunctions) + ordinal - pExport->Base;
    result = *rvaPtr;
    newRvaValue = (DWORD)functionAddress - (DWORD)object->moduleHandler;
    HackWriteProcessMemory(GetCurrentProcess(), rvaPtr, &newRvaValue, sizeof(DWORD), &writeLen);

    return (FARPROC)rva2ptr(object, result);
  }
  return NULL;
}
