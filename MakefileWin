SRCDIR=./src
INCDIR=./include
#LIBS = C:\ProjectCourseSnomos\include
INCLUDE = -IC:\msys2\mingw64\include\SDL2 -I$(INCDIR)
CFLAGS = -g -I$(INCLUDE) -c 
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lsdL2_image -mwindows -lm
CC = gcc

snomos: main.o character.o
	@echo "Building Snomos"
	$(CC) -o snomos main.o character.o $(LDFLAGS)

main.o: $(SRCDIR)/main.c $(INCDIR)/character.h
	$(CC) $(CFLAGS) $(SRCDIR)/main.c 

character.o: $(SRCDIR)/character.c $(INCDIR)/character.h
	$(CC) $(CFLAGS) $(SRCDIR)/character.c 

clean: 
	rm *.o
	rm *.exe

