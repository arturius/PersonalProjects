all: test mapMaker pathFind

mapMaker: mapMaker.c DnD.o DnD.h
	gcc -Wall -std=c99 -pedantic  mapMaker.c DnD.o -o mapMaker -lm

pathFind: pathFind.c DnD.o DnD.h
	gcc -Wall -std=c99 -pedantic  pathFind.c DnD.o -o pathFind -lm

test: test.c DnD.o DnD.h
	gcc -Wall -std=c99 -pedantic  test.c DnD.o -o test -lm

DnD.o: DnD.c DnD.h
	gcc -Wall -std=c99 -pedantic -c DnD.c -o DnD.o -lm
