
OBJS=scanner.o parser.o compiler.o ast.o list.o cmd.o utility.o threeAddressCode.o mips.o hashtable.o scope.o
INTERM=scanner.c parser.c parser.h
PROGRAM=./compiler
CFLAGS=-g 

all: $(PROGRAM)

scanner.c: scanner.flex parser.h
	flex -o scanner.c scanner.flex

parser.c parser.h: parser.bison
	bison --defines=parser.h -o parser.c parser.bison

$(PROGRAM): $(OBJS)
	$(CC) -Wall -g -o $(PROGRAM) $(OBJS)
clean:
	rm -f $(PROGRAM) $(OBJS) $(INTERM)

