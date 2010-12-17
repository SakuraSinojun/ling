





CC	= @gcc
CXX	= @g++
LD	= @g++



CFLAGS  = -c -O -Werror -Wall
LFTRGT  = -luser32 -lgdi32 -lwinmm -lmsimg32
LFRNDR  = -lopengl32 -lglu32 -luser32 -lgdi32 -lwinmm -shared

COUT	= -o
LOUT	= -o

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





