CFLAGS= -Wall -fpic -coverage -lm -std=c99

testme: testme.c
	gcc -o testme testme.c -g $(CFLAGS)
	./testme

all: testme 

clean:
	rm -f *.o testme.exe testme *.gcov *.gcda *.gcno *.so *.out
