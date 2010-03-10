#ifndef CARBON_COMMON_H
#define CARBON_COMMON_H

#include <xcb/xcb.h>

/* TODO: Refractor this. I'm not sure where this belongs to logically, so
 * I'm breaking it out into its own file for now.
 *
 * Ultimately, some of this will need to belong to the buffer, and some to the terminal,
 * and some to both, data passed from one to the other */

/* Attribute, Cursor, Character state, Terminal mode, Screen draw mode */

#define DefaultFG 7
#define DefaultBG 0
#define DefaultCS 1
#define BellCol   DefaultFG

enum { ATnone=0 , ATreverse=1 , ATunderline=2, ATbold=4 };
enum { CSup, CSdown, CSright, CSleft, CShide, CSdraw, CSwrap, CSsave, CSload };
enum { CRset=1, CRupdate=2 };
enum { TMwrap=1, TMinsert=2 };

typedef int color_t;

typedef struct {
	char mode;        /* attribute flags */
	color_t fg;       /* foreground */
	color_t bg;       /* background */
	char state;       /* state flag */
} attr_t;

typedef struct {
	attr_t attr;
	char c;           /* character code */
} glyph_t;

typedef glyph_t *line_t;

typedef struct {
	attr_t attr;      /* current character attributes */
	char hidden;
	int x;
	int y;
} cursor_t;

typedef struct {
	xcb_connection_t *conn;
	xcb_window_t win;
	int str;
	int width;
	int height;
	int charwidth;
	int charheight;
} xwindow_t;

typedef struct {
	xcb_gcontext_t gc;
} dc_t;

#endif
