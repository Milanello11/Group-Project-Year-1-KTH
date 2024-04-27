SRCDIR=./src
COMMONSRCDIR=../lib/src
INCDIR=../lib/include
#LIBS = C:\ProjectCourseSnomos\include
INCLUDE = -IC:\msys2\mingw64\include\SDL2 -I$(INCDIR)
CFLAGS = -g $(INCLUDE) -c 
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net -lSDL2_ttf -lm
CC = gcc

snomosClient: main.o character.o snowball.o map.o menu.o button.o text.o
	@echo "Building Snomos"
	$(CC) -o snomosClient main.o character.o snowball.o map.o menu.o button.o text.o $(LDFLAGS)

main.o: $(SRCDIR)/main.c $(INCDIR)/character.h $(INCDIR)/characterData.h $(INCDIR)/snowball.h $(INCDIR)/map.h
	$(CC) $(CFLAGS) $(SRCDIR)/main.c 

character.o: $(COMMONSRCDIR)/character.c $(INCDIR)/character.h $(INCDIR)/characterData.h $(INCDIR)/snowball.h $(INCDIR)/map.h
	$(CC) $(CFLAGS) $(COMMONSRCDIR)/character.c

snowball.o: $(COMMONSRCDIR)/snowball.c $(INCDIR)/snowball.h $(INCDIR)/characterData.h
	$(CC) $(CFLAGS) $(COMMONSRCDIR)/snowball.c 

map.o: $(COMMONSRCDIR)/map.c $(INCDIR)/map.h
	$(CC) $(CFLAGS) $(COMMONSRCDIR)/map.c

menu.o: $(COMMONSRCDIR)/menu.c $(INCDIR)/menu.h
	$(CC) $(CFLAGS) $(COMMONSRCDIR)/menu.c

button.o: $(COMMONSRCDIR)/button.c $(INCDIR)/button.h
	$(CC) $(CFLAGS) $(COMMONSRCDIR)/button.c

text.o: $(COMMONSRCDIR)/text.c $(INCDIR)/text.h
	$(CC) $(CFLAGS) $(COMMONSRCDIR)/text.c