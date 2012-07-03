#include <windows.h>
#include <shellapi.h>
#include <tchar.h>
#include <assert.h>
#include <stdio.h>
#include "../inc/common.h"

#include "demo.h"
#pragma comment(lib, "shell32.lib")


void demo_browser(void* arg)
{
  const TCHAR* option  = TEXT("open");
  const TCHAR* address = TEXT("www.oschina.net");
  HINSTANCE hResult;

  UNUSED_PARAM(arg)

  hResult = ShellExecute(NULL, option, address, NULL, NULL, SW_SHOWNORMAL);
  assert(hResult > (HINSTANCE)HINSTANCE_ERROR);
}
