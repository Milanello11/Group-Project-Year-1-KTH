SRCDIR=./src
COMMONSRCDIR=../lib/src
INCDIR=../lib/include
#LIBS = C:\ProjectCourseSnomos\include
INCLUDE = -IC:\msys2\mingw64\include\SDL2 -I$(INCDIR)
CFLAGS = -g $(INCLUDE) -c 
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net -lm
CC = gcc

snomosServer: main.o character.o snowball.o map.o
	@echo "Building Snomos"
	$(CC) -o snomosServer main.o character.o snowball.o map.o $(LDFLAGS)

main.o: $(SRCDIR)/main.c $(INCDIR)/character.h $(INCDIR)/characterData.h $(INCDIR)/snowball.h $(INCDIR)/map.h
	$(CC) $(CFLAGS) $(SRCDIR)/main.c 

character.o: $(COMMONSRCDIR)/character.c $(INCDIR)/character.h $(INCDIR)/characterData.h $(INCDIR)/snowball.h $(INCDIR)/map.h
	$(CC) $(CFLAGS) $(COMMONSRCDIR)/character.c

snowball.o: $(COMMONSRCDIR)/snowball.c $(INCDIR)/snowball.h $(INCDIR)/character.h
	$(CC) $(CFLAGS) $(COMMONSRCDIR)/snowball.c 

map.o: $(COMMONSRCDIR)/map.c $(INCDIR)/map.h
	$(CC) $(CFLAGS) $(COMMONSRCDIR)/map.c

clean: 
	del *.o
	del *.exe