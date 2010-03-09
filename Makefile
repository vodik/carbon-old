CC=clang
CFLAGS=-Wall -pedantic `pkg-config --cflags xcb xcb-keysyms` --std=c99 -g
LDFLAGS=`pkg-config --libs xcb xcb-keysyms`

SRC = tty.c util.c buffer.c key.c carbon.c autil.c
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
