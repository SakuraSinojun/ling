

CC = @gcc
LD = @g++
CXX = @g++




CFLAGS = -c -O -Wall -Werror 
LDFLAGS = -lgdi32 -luser32 -lmsimg32 -lwinmm -lopengl32 -lglu32
CCOUT = -o 
LDOUT = -o 



OBJS = test.o \
	preload.o \
	datafile.o \
	error.o \
	render.o
	


TARGET = test.exe
GLRENDER = glrender.dll



all: $(TARGET) $(GLRENDER) clean

$(TARGET): $(OBJS) glrender.a
	@echo Linking $@ ...
	$(LD) $(OBJS) debug/glrender.a $(LDFLAGS) $(LDOUT)debug/$@ 
	@echo ---------------------------------------
	@echo all done.

glrender.a: $(GLRENDER)

$(GLRENDER): glrender.o
	@echo Linking $@ ...
	$(LD) -shared glrender.o -Wl,--out-implib,debug/glrender.a $(LDFLAGS) $(LDOUT)debug/$@ 
	@echo ---------------------------------------
	@echo all done.


glrender.o: glrender.c glrender.h
	@echo Compling $< ...
	$(CXX) $< $(CFLAGS) $(CCOUT)$@ 
	@echo ---------------------------------------
	
.cpp.o:
	@echo Compling $< ...
	$(CXX) $< $(CFLAGS) $(CCOUT)$@ 
	@echo ---------------------------------------

.c.o:
	@echo Compling $< ...
	$(CC) $< $(CFLAGS) $(CCOUT)$@ 
	@echo ---------------------------------------


rebuild: clean $(TARGET)


.PHONY: clean

clean:
	del /Q *.o

	

