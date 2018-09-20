all: test mapMaker pathFind

mapMaker: mapMaker.c DnD.o DnD.h
	gcc -Wall -ansi -pedantic  mapMaker.c DnD.o -o mapMaker -lm

pathFind: pathFind.c DnD.o DnD.h
	gcc -Wall -ansi -pedantic  pathFind.c DnD.o -o pathFind -lm

test: test.c DnD.o DnD.h
	gcc -Wall -ansi -pedantic  test.c DnD.o -o test -lm

DnD.o: DnD.c DnD.h
	gcc -Wall -ansi -pedantic -c DnD.c -o DnD.o -lm
