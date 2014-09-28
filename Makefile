vpath %.c src
vpath %.h src

CPPFLAGS = -g --std=gnu99 -I.

3150sh: main.c parser.o interpreter.o
	gcc -g $^ -o $@

interpreter.o: parser.h

test: test.c parser.o

.PHONY: clean

clean:
	@rm *.o 3150sh 2> /dev/null || true
