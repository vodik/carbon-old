#ifndef CARBON_BUFFER_H
#define CARBON_BUFFER_H

#include "tty.h"

typedef struct termbuf_t termbuf_t;

void termbuf_putchar(void *, char c);
void termbuf_puttab(void *);
void termbuf_putcr(void *);
void termbuf_putnl(void *);

void termbuf_insertblank(void *, int n);
void termbuf_insertblankline(void *, int n);
void termbuf_deletechar(void *, int n);
void termbuf_moveto(void *, int x, int y);
void termbuf_clearregion(void *, int x1, int y1, int x2, int y2);

void termbuf_savecursor(void *);
void termbuf_loadcursor(void *);

static const buf_t termbuf_impl = {
	.putchar         = termbuf_putchar,
	.puttab          = termbuf_puttab,
	.putcr           = termbuf_putcr,
	.putnl           = termbuf_putnl,

	.insertblank     = termbuf_insertblank,
	.insertblankline = termbuf_insertblankline,
	.deletechar      = termbuf_deletechar,
	.moveto          = termbuf_moveto,
	.clearregion     = termbuf_clearregion,

	.savecursor      = termbuf_savecursor,
	.loadcursor      = termbuf_loadcursor,
};

#endif
