#define STRICT
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "../inc/common.h"

#include "demo.h"



typedef UINT (CALLBACK *pPROC)(void);


void demo_gdiquerytable_test(void* arg)
{
  pPROC p;

  UNUSED_PARAM(arg)
  fprintf(stdout, "call function : %s\n", __FUNCTION__);

  p = (pPROC)GetProcAddress(GetModuleHandle(TEXT("GDI32.DLL")), "GdiQueryTable");
  if (NULL != p)
  {
    TCHAR buffer[32];
    wsprintf(buffer, TEXT("%8lX"), p());
    MessageBox(NULL, buffer, TEXT("GdiQueryTable() returns"), MB_OK);
  }
}
