#include <stdio.h>
#include <string.h>
#include "../inc/common.h"

#include "demo.h"

struct WDemo {
  char* cmd;
  char* fullCmd;
  void (*demo)(void*);
};

static void demo_help(void* arg)
{
  UNUSED_PARAM(arg)

  fprintf(stdout, 
    "wgraph demo version 0.1 - Copyright (c) 2012 ASMlover.\n"
    "usage : wgraph [option]\n\n"
    "General options:\n"
    "  -[h]elp      display the help document of the wgraph.\n"
    "  -[b]rowser   display the demo of run web browser.\n"
    );
}

int 
main(int argc, char* argv[], char* envp[])
{
  static struct WDemo demos[] = {
    { "-h", "-help",    demo_help     }, 
    { "-b", "-browser", demo_browser  }, 
  };

  UNUSED_PARAM(envp)
  if (argc < 2)
    demo_help(NULL);
  else
  {
    int i, found = 0;
    for (i = 0; i < countof(demos); ++i)
    {
      if (0 == stricmp(argv[1], demos[i].cmd) 
        || 0 == stricmp(argv[1], demos[i].fullCmd))
      {
        found = 1;
        demos[i].demo((3 == argc) ? argv[2] : NULL);
        break;
      }
    }

    if (!found)
      demo_help(NULL);
  }

  return 0;
}
