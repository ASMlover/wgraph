/* 
 * Copyright (c) 2012, ASMlover. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list ofconditions and the following disclaimer.
 * 
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
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
    "  -[d]esktop   display the demo of draw line on desktop.\n"
    "  -[f]ullwin   display the demo of full window\n"
    "  -[dx]win     display the demo of DirectShow window (DX)\n"
    "  -[t]imer     display the demo of wgTimer module\n"
    "  -[peh]ack    display the demo of wgPEHack module\n"
    );
}

int 
main(int argc, char* argv[], char* envp[])
{
  static struct WDemo demos[] = {
    { "-h",   "-help",    demo_help         }, 
    { "-b",   "-browser", demo_browser      }, 
    { "-d",   "-desktop", demo_drawdesktop  }, 
    { "-f",   "-fullwin", demo_fullwindow   }, 
    { "-dx",  "-dxwin",   demo_windowdx     }, 
    { "-t",   "-timer",   demo_timer        }, 
    { "-peh", "-pehack",  demo_pehack       }, 
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
