





CC	= @cl
CXX	= @cl
LD	= @link



CFLAGS  = /nologo /c /W3 /O2 /WX
LFTRGT  = user32.lib gdi32.lib winmm.lib msimg32.lib
LFRNDR  = user32.lib gdi32.lib winmm.lib msimg32.lib /DLL

COUT	= /Fo
LOUT	= /OUT:

OBJS =  fireball.o scriptheader.o


fireball: fireball.o scriptheader.o
	@echo Linking fireball.dll
	$(LD) fireball.o scriptheader.o $(LFRNDR) $(LOUT)fireball.dll
	@echo ---------------------------------

.cpp.o:
	@echo Compling $@ ...
	$(CXX) $< $(CFLAGS) $(COUT)$@
	@echo ---------------------------------
	
.c.o:
	@echo Compling $@ ...
	$(CC) $< $(CFLAGS) $(COUT)$@
	@echo ---------------------------------

.PHONY: all clean

clean:
	del *.o *.exp *.lib /Q





