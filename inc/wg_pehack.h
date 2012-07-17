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
#ifndef __WG_PEHACK_HEADER_H__
#define __WG_PEHACK_HEADER_H__

struct wgPEHack;

extern struct wgPEHack* wgPEHackCreate(HMODULE moduleHandler);
extern void wgPEHackRelease(struct wgPEHack** object);

extern const void* wgPEHackGetDirectory(struct wgPEHack* object, int id);

/* returns PIMAGE_IMPORT_DESCRIPTOR for an imported module */
extern PIMAGE_IMPORT_DESCRIPTOR wgPEHackGetImportDescriptor(struct wgPEHack* object, LPCSTR dllName);

/* returns address of __imp_xxx variable for an import function */
extern const UINT* wgPEHackGetFunctionPtr(struct wgPEHack* object, PIMAGE_IMPORT_DESCRIPTOR import, LPCSTR functionName);
extern FARPROC wgPEHackSetImportAddress(struct wgPEHack* object, LPCSTR dllName, LPCSTR functionName, FARPROC functionAddress);
extern FARPROC wgPEHackSetExportAddress(struct wgPEHack* object, LPCSTR functionName, FARPROC functionAddress);

#endif  /* __WG_PEHACK_HEADER_H__ */
