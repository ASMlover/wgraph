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
#ifndef __PE_FILE_HEADER__
#define __PE_FILE_HEADER__

struct PEFile;

extern struct PEFile* peFileCreate(HMODULE hModule);
extern void peFileRelease(struct PEFile** object);

extern const void* peFileGetDirectory(struct PEFile* object, int id);

/* returns PIMAGE_IMPORT_DESCRIPTOR for an imported module */
extern PIMAGE_IMPORT_DESCRIPTOR peFileGetImportDiscriptor(struct PEFile* object, LPCSTR dllName);

/* returns address of __imp_xxx variable for an import function */
extern const unsigned int* peFileGetFunctionPtr(struct PEFile* object, PIMAGE_IMPORT_DESCRIPTOR pImport, LPCSTR functionName);
extern FARPROC peFileSetImportAddress(struct PEFile* object, LPCSTR dllName, LPCSTR functionName, FARPROC functionAddress);
extern FARPROC peFileSetExportAddress(struct PEFile* object, LPCSTR functionName, FARPROC functionAddress);

#endif  /* __PE_FILE_HEADER__ */
