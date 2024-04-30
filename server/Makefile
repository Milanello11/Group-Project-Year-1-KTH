SRCDIR=./src
COMMONSRCDIR=../lib/src
INCDIR=../lib/include
#LIBS = C:\ProjectCourseSnomos\include
INCLUDE = -IC:\msys2\mingw64\include\SDL2 -I$(INCDIR)
CFLAGS = -g $(INCLUDE) -c 
LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_net -lSDL2_ttf -lm
CC = gcc

snomosServer: main.o character.o snowball.o map.o text.o
	@echo "Building Snomos"
	$(CC) -o snomosServer main.o character.o snowball.o map.o text.o $(LDFLAGS)

main.o: $(SRCDIR)/main.c $(INCDIR)/character.h $(INCDIR)/characterData.h $(INCDIR)/snowball.h $(INCDIR)/map.h $(INCDIR)/text.h
	$(CC) $(CFLAGS) $(SRCDIR)/main.c 

character.o: $(COMMONSRCDIR)/character.c $(INCDIR)/character.h $(INCDIR)/characterData.h
	$(CC) $(CFLAGS) $(COMMONSRCDIR)/character.c

snowball.o: $(COMMONSRCDIR)/snowball.c $(INCDIR)/snowball.h
	$(CC) $(CFLAGS) $(COMMONSRCDIR)/snowball.c 

map.o: $(COMMONSRCDIR)/map.c $(INCDIR)/map.h
	$(CC) $(CFLAGS) $(COMMONSRCDIR)/map.c

text.o: $(COMMONSRCDIR)/text.c $(INCDIR)/text.h
	$(CC) $(CFLAGS) $(COMMONSRCDIR)/text.c

clean: 
	rm *.o
	rm *.exe