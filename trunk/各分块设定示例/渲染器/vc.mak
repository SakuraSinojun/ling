

CC = @cl
LD = @link
CXX = @cl



CFLAGS = /c /O2 /nologo
LDFLAGS = /nologo gdi32.lib user32.lib msimg32.lib winmm.lib opengl32.lib glu32.lib
CCOUT = /Fo
LDOUT = /OUT:



OBJS = test.o \
	preload.o \
	datafile.o \
	error.o \
	render.o
	


TARGET = test.exe
GLRENDER = glrender.dll



all: $(TARGET) $(GLRENDER) clean

$(TARGET): $(OBJS) glrender.lib
	@echo Linking $@ ...
	$(LD) $(OBJS) debug/glrender.lib $(LDFLAGS) $(LDOUT)debug/$@ 
	@echo ---------------------------------------
	@echo all done.

glrender.lib: $(GLRENDER)

$(GLRENDER): glrender.o
	@echo Linking $@ ...
	$(LD) glrender.o $(LDFLAGS) $(LDOUT)debug/$@ /DLL /IMPLIB:debug/glrender.lib
	@echo ---------------------------------------
	@echo all done.



	
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

	

