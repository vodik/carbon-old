#ifndef CARBON_TERM_H
#define CARBON_TERM_H

typedef struct tty_t tty_t;
typedef void (*shellfn)();

typedef struct buf_t {
	void (*putchar)(void *, char c);

	void (*puttab)(void *);
	void (*putcr)(void *);
	void (*putnl)(void *);

	void (*insertblank)(void *, int n);
	void (*insertblankline)(void *, int n);
	void (*deletechar)(void *, int n);
	void (*moveto)(void *, int x, int y);
	void (*clearregion)(void *, int x1, int y1, int x2, int y2);

	void (*savecursor)(void *);
	void (*loadcursor)(void *);
} buf_t;

tty_t *tty_create(int rows, int cols, shellfn shell, const buf_t *bufimpl, void *buf);
void tty_destroy(tty_t *);
void tty_resize(tty_t *, int rows, int cols);

void tty_read(tty_t *);
void tty_send(tty_t *, const char *str, int len);

int tty_getfd(tty_t *);

#endif
