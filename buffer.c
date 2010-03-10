#include "buffer.h"

#include <stdlib.h>
#include <stdio.h>
#include <xcb/xcb.h>
#include <assert.h>

#include "common.h"
#include "config.h"

struct termbuf_t {
	int rows;
	int cols;
	int top;                /* top scroll limit */
	int bottom;             /* bottom scroll limit */
	line_t *lines;          /* the screen/history */
	cursor_t cursor;        /* the cursor */
	int mode;               /* terminal mode */
};

/*****************************************************************************/

/* TODO: check cookie */
void xclear(xwindow_t *xw, int x1, int y1, int x2, int y2)
{
	xcb_void_cookie_t cookie;
	cookie = xcb_clear_area_checked(xw->conn,
			0,
			xw->win,
			x1 * xw->charwidth,
			y1 * xw->charheight,
			(x2 - x1 + 1) * xw->charwidth,
			(y2 - y1 + 1) * xw->charheight);
}

/* TODO: check cookie */
void xscroll(xwindow_t *xw, dc_t *dc, termbuf_t *b)
{
	xcb_void_cookie_t cookie;
	int srcy   = (b->top + 1) * xw->charheight;
	int dsty   = b->top * xw->charheight;
	int height = (b->bottom - b->top) * xw->charheight;

	//xcursor(CShide);
	cookie = xcb_copy_area_checked(xw->conn,
			xw->win,
			xw->win,
			dc->gc,
			0,
			srcy,
			0,
			dsty,
			xw->width,
			height);
	xclear(xw, 0, b->bottom, b->cols - 1, b->bottom);
}

/* TODO: seperate attributes from the glyph, then combine into glyph. that way this is called
 * with attr_t. Its the only reason why we pass around base. It be more intuitive for understanding
 * the code by looking at it */
void xdrawbuf(char *buf, attr_t base, int x, int y, int len)
{
}

/*****************************************************************************/

#define MAXDRAWBUF 1024
void termbuf_draw(termbuf_t *b, xwindow_t *xw, dc_t *dc)
{
	int i, x, y, ox;
	attr_t base;
	glyph_t new;
	char buf[MAXDRAWBUF];

	for (y = 0; y < b->rows; ++y) {
		base = b->lines[y][0].attr;
		i = ox = 0;
		for (x = 0; x < b->cols; ++x) {
			new = b->lines[y][x];
			/* keep filling the buffer as long as the attribute is equal */
			/* TODO: implement attributes */
			if (/*!ATTRCMP(base, new) && */i < MAXDRAWBUF)
				buf[i++] = new.c;
			else {
				xdrawbuf(buf, base, ox, y, i);
				buf[0] = new.c;
				i = 1;
				ox = x;
				base = new.attr;
			}
		}
		/* draw out the residual */
		xdrawbuf(buf, base, ox, y, i);
	}
	/* TODO draw the cursor */
}

/* TODO: split this into 4 different functions. Why do we need a switch? */
static void termbuf_movecursor(termbuf_t *b, int dir)
{
	int newx = b->cursor.x;
	int newy = b->cursor.y;

	switch (dir) {
		case CSup:
			--newy;
			break;
		case CSdown:
			++newy;
			break;
		case CSleft:
			if(--newx < 0) {
				newx = b->cols - 1;
				--newy;
				if (newy < b->top)
					newy = b->top;
			}
			break;
		case CSright:
			if(++newx < b->cols) {
				/* TODO */
			}
			break;
	}
	termbuf_moveto(b, newx, newy);
}

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

	int x = b->cursor.x;
	int y = b->cursor.y;
	glyph_t *glyph = &b->lines[y][x];

	glyph->attr = b->cursor.attr;
	glyph->c = c;
	glyph->attr.state |= CRset | CRupdate;
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
