# this is just passed as parameters to the compiler
# lizzie: you can think of it like "gcc $CFLAGS -c mything.c -o mything.o"
CFLAGS=-Wall -g -fsanitize=address

# http://stackoverflow.com/questions/170467/makefiles-compile-all-c-files-at-once
SRCS = $(wildcard *.c)

PROGS = $(patsubst %.c,%,$(SRCS))

all: $(PROGS)

clean:
	rm -f $(PROGS)
	rm -rf *.dSYM
