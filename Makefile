vpath %.c src
vpath %.h src

dumbsh: main.c parser.o interpreter.o
	gcc -g $^ -o $@
parser.o: parser.c
	gcc -g --std=gnu99 -I. -c $<
interpreter.o: interpreter.c
	gcc -g --std=gnu99 -I. -c $<
