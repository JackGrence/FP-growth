CC = gcc
CFLAGS = -Wall -O3
OBJECTS = main.o linklist.o heap_sort.o display.o fp_tree.o
LIBS =

all: fp_growth

debug: CFLAGS = -ggdb

debug: fp_growth

windows: CC = x86_64-w64-mingw32-gcc # for windows

windows: fp_growth

fp_growth: ${OBJECTS}
	${CC} ${CFLAGS} ${OBJECTS} ${LIBS} -o $@

main.o: linklist.h heap_sort.h display.h fp_tree.h

linklist.o: linklist.h

heap_sort.o: heap_sort.h

display.o: display.h linklist.h heap_sort.h fp_tree.h

fp_tree.o: fp_tree.h heap_sort.h

clean:
	-rm *.o
