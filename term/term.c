/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) Simon Gomizelj, 2011
 */

#include "term.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cairo.h>
#include <assert.h>

struct carbon_term {
	cairo_t *cr;
	cairo_font_extents_t fe;
	int x, y;
	int w, h;
	int px, py;
};

carbon_term_t *
carbon_new_term(cairo_surface_t *surface, const char *font, unsigned size)
{
	assert(surface);

	/* create our terminal */
	int width  = cairo_image_surface_get_width(surface);
	int height = cairo_image_surface_get_height(surface);
	cairo_t *cr = cairo_create(surface);

	/* set a nice background */
	cairo_rectangle(cr, 0.0, 0.0, width, height);
	cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
	cairo_fill(cr);

	cairo_select_font_face(cr, font, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, size);
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);

	carbon_term_t *term = malloc(sizeof(carbon_term_t));
	term->cr = cr;
	cairo_font_extents(cr, &term->fe);

	term->x = 0;
	term->y = 0;
	term->w = width / term->fe.max_x_advance;
	term->h = height / term->fe.max_y_advance;
	term->px = 0;
	term->py = term->fe.height;

	return term;
}

void
carbon_term_destroy(carbon_term_t *term)
{
	cairo_destroy(term->cr);
	free(term);
}

void
carbon_term_print(carbon_term_t *term, const char *msg)
{
	int len = strlen(msg), i;
	char str[2] = { 0 };

	for (i = 0; i < len; i++) {
		if (msg[i] == '\n' || ++term->x > term->w - 1) {
			if (msg[i] == '\n')
				++i;
			term->x = 0;
			term->px = 0;
			++term->y;
			term->py += term->fe.height;
		}
		str[0] = msg[i];
		cairo_move_to(term->cr, term->px, term->py);
		cairo_show_text(term->cr, str);
		term->px += term->fe.max_x_advance;
	}
}
