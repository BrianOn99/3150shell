vpath %.c src
vpath %.h src

CPPFLAGS = -g --std=gnu99 -I.

dumbsh: main.c parser.o interpreter.o
	gcc -g $^ -o $@

test: test.c parser.o

.PHONY: clean

clean:
	rm *.o dumbsh
