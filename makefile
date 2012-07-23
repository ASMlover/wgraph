BIN		= wgraph.exe
CC		= cl -c -nologo
CFLAG	= -O2 -W3 -MD -GS -Zi -Fd"vc.pdb" -D_DEBUG\
				-D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_WARNINGS
LINK	= link -nologo
LFLAG	= -DEBUG -PDB:"wgraph.pdb" -manifest -manifestfile:$(BIN).manifest\
				-manifestuac:no kernel32.lib user32.lib gdi32.lib
MT		= mt -nologo
RM		= rm
SRCS	= ./demo/demo.c ./demo/demo_browser.c ./demo/demo_drawdesktop.c\
				./demo/demo_fullwindow.c ./demo/demo_tools.c ./demo/demo_windowdx.c\
				./demo/demo_timer.c ./demo/demo_pehack.c ./demo/demo_gdi.c\
				./demo/demo_gdi_test.c ./demo/demo_gdiquerytable_test.c\
				\
				./src/wg_window.c ./src/wg_timer.c ./src/wg_pehack.c
OBJS	= demo.obj demo_browser.obj demo_drawdesktop.obj demo_fullwindow.obj\
				demo_tools.obj demo_windowdx.obj demo_timer.obj demo_pehack.obj\
				demo_gdi.obj demo_gdi_test.obj demo_gdiquerytable_test.obj\
				\
				wg_window.obj wg_timer.obj wg_pehack.obj

$(BIN):$(OBJS)
	$(LINK) -out:$(BIN) $(OBJS) $(LFLAG)
	$(MT) -manifest $(BIN).manifest -outputresource:$(BIN);1

$(OBJS):$(SRCS)
	$(CC) $(CFLAG) $(SRCS)

clean:
	$(RM) $(BIN) *.obj *.manifest *.ilk *.pdb
