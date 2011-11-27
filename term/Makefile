CC = gcc
CFLAGS = -Wall -pedantic -std=gnu99 `pkg-config --cflags cairo`
LDFLAGS = `pkg-config --libs cairo`

OBJ = term.o main.o

all: term

term: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

%.o: %.c
	@echo CC $@
	@${CC} -o $@ -c $< ${CFLAGS}

clean:
	@echo cleaning up...
	@rm *.o term

.PHONY: all clean
