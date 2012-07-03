BIN		= wgraph.exe
CC		= cl -c -nologo
CFLAG	= -O2 -W4 -MD -GS -Zi -Fd"vc.pdb" -D_DEBUG\
				-D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_WARNINGS
LINK	= link -nologo
LFLAG	= -DEBUG -PDB:"wgraph.pdb" -manifest -manifestfile:$(BIN).manifest\
				-manifestuac:no kernel32.lib user32.lib gdi32.lib
MT		= mt -nologo
RM		= rm
SRCS	= ./demo/demo.c ./demo/demo_browser.c
OBJS	= demo.obj demo_browser.obj

$(BIN):$(OBJS)
	$(LINK) -out:$(BIN) $(OBJS) $(LFLAG)
	$(MT) -manifest $(BIN).manifest -outputresource:$(BIN);1

$(OBJS):$(SRCS)
	$(CC) $(CFLAG) $(SRCS)

clean:
	$(RM) $(BIN) *.obj *.manifest *.ilk *.pdb
