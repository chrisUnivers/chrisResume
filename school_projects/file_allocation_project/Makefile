#my makefile

CC=gcc

all: diskinfo disklist diskget

diskinfo: diskinfo.o
	$(CC) diskinfo.o -o diskinfo

diskinfo.o: diskinfo.c
	$(CC) -Wall -c diskinfo.c

disklist: disklist.o
	$(CC) disklist.o -o disklist

disklist.o: disklist.c
	$(CC) -Wall -c disklist.c

diskget: diskget.o dirNodes.o
	$(CC) diskget.o dirNodes.o -o diskget

diskget.o: diskget.c dirNodes.h
	$(CC) -Wall -c diskget.c

dirNodes.o: dirNodes.c dirNodes.h
	$(CC) -c dirNodes.c

clean:
	rm -rf *.o diskinfo disklist diskget