CC=clang
CFLAGS=-Wall -pedantic `pkg-config --cflags xcb` --std=c99 -g
LDFLAGS=`pkg-config --libs xcb`

SRC = tty.c util.c buffer.c carbon.c
OBJ = ${SRC:.c=.o}

all: carbon

.c.o:
	@echo CC $@
	@${CC} -c ${CFLAGS} $<

carbon: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	@echo cleaning...
	@rm -f carbon ${OBJ}
