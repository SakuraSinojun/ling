


CC      = @gcc
CXX     = @g++
LD      = @g++

CFLAGS  = -c -O -Werror -Wall
LFTRGT  = -luser32 -lgdi32 -lwinmm -lmsimg32
LFRNDR  = -lopengl32 -lglu32 -luser32 -lgdi32 -lwinmm -shared

COUT	= -o
LOUT	= -o

VPATH = .\game: .\common: .\win32: .\glrender
OUTPATH = ..\release

TARGET = $(OUTPATH)\ling.exe
RENDER = $(OUTPATH)\glrender.dll

#$(wildcard *.o)


OBJS = 	error.o \
	datafile.o \
	preload.o \
	main.o \
	Creature.o \
	Game.o \
	Hero.o \
	Input.o \
	Mod.o \
	Monster.o \
	Npc.o \
	render.o


all: $(RENDER) $(TARGET)

$(TARGET): $(OBJS)
	@echo Linking $@ ...
	$(LD) $(OBJS) $(LFTRGT) $(LOUT)$@
	@echo ---------------------------------
	@echo done.

$(RENDER): glrender.o
	@echo Linking $@ ...
	$(LD) glrender.o $(LFRNDR) $(LOUT)$@
	@echo ---------------------------------
	@echo done.

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
	del *.o /Q
















