#include "buffer.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void termbuf_putchar(void *vb, char c)
{
	assert(vb);
	termbuf_t *b = vb;
}

void termbuf_puttab(void *vb)
{
	assert(vb);
	termbuf_t *b = vb;
}

void termbuf_putcr(void *vb)
{
	assert(vb);
	termbuf_t *b = vb;
}

void termbuf_putnl(void *vb)
{
	assert(vb);
	termbuf_t *b = vb;
}

void termbuf_insertblank(void *vb, int n)
{
	assert(vb);
	termbuf_t *b = vb;
}

void termbuf_insertblankline(void *vb, int n)
{
	assert(vb);
	termbuf_t *b = vb;
}

void termbuf_deletechar(void *vb, int n)
{
	assert(vb);
	termbuf_t *b = vb;
}

void termbuf_moveto(void *vb, int x, int y)
{
	assert(vb);
	termbuf_t *b = vb;
}

void termbuf_clearregion(void *vb, int x1, int y1, int x2, int y2)
{
	assert(vb);
	termbuf_t *b = vb;
}

void termbuf_savecursor(void *vb)
{
	assert(vb);
	termbuf_t *b = vb;
}

void termbuf_loadcursor(void *vb)
{
	assert(vb);
	termbuf_t *b = vb;
}
