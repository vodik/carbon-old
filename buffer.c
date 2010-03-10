#include "buffer.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "common.h"
#include "config.h"

struct termbuf_t {
	int rows;
	int cols;

	line_t *lines;          /* the screen/history */
	cursor_t cursor;        /* the cursor */

	int top;                /* top scroll limit */
	int bottom;             /* bottom scroll limit */
	int mode;               /* terminal mode */
};

termbuf_t *termbuf_create(int rows, int cols)
{
	termbuf_t *b = malloc(sizeof(termbuf_t));
	int row;

	b->rows = rows, b->cols = cols;
	b->top = 0, b->bottom = b->rows - 1;

	/* mode */
	b->mode = TMwrap;

	/* cursor */
	b->cursor.attr.mode = ATnone;
	b->cursor.attr.fg = DefaultFG;
	b->cursor.attr.bg = DefaultBG;
	b->cursor.x = b->cursor.y = 0;
	b->cursor.hidden = 0;

	/* allocate screen */
	b->lines = calloc(b->rows, sizeof(line_t));
	for(row = 0 ; row < b->rows; row++)
		b->lines[row] = calloc(b->cols, sizeof(glyph_t));

	return b;
}

void termbuf_putchar(void *vb, char c)
{
	assert(vb);
	termbuf_t *b = vb;
}

void termbuf_puttab(void *vb)
{
	assert(vb);
	termbuf_t *b = vb;

	int spaces = tabwidth - b->cursor.x % tabwidth;
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
