BIN	= wgraph.exe
OUT	= bin
CC	= cl -c -nologo
CFLAG	= -O2 -W3 -MD -GS -Zi -Fd"vc.pdb" -D_DEBUG\
	-D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_WARNINGS
LINK	= link -nologo
LFLAG	= -DEBUG -PDB:"wgraph.pdb" -manifest -manifestfile:$(BIN).manifest\
	-manifestuac:no kernel32.lib user32.lib gdi32.lib
MT	= mt -nologo
OBJS	= demo.obj demo_browser.obj demo_drawdesktop.obj demo_fullwindow.obj\
	demo_tools.obj demo_windowdx.obj demo_timer.obj demo_pehack.obj\
	demo_gdi.obj demo_gdi_test.obj demo_gdiquerytable_test.obj\
	demo_gditable_test.obj demo_gditable.obj demo_freakout.obj\
	\
	wg_window.obj wg_timer.obj wg_pehack.obj wg_draw.obj
RM	= del
MD	= mkdir
RD	= rd /s /q
CP	= copy


all: bin 

bin: $(BIN)

rebuild: clean all

install:
	if not exist $(OUT) $(MD) $(OUT)
	$(CP) $(BIN) $(OUT)

uninstall:
	if exist $(OUT) $(RD) $(OUT)

clean:
	$(RM) $(OBJS) $(BIN) *.manifest *.ilk *.pdb



$(BIN): $(OBJS)
	$(LINK) -out:$(BIN) $(OBJS) $(LFLAG)
	$(MT) -manifest $(BIN).manifest -outputresource:$(BIN);1

{.\demo}.c{}.obj:
	$(CC) $(CFLAG) $<

{.\src}.c{}.obj:
	$(CC) $(CFLAG) $<
