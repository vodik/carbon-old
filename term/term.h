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

#ifndef TERM_H
#define TERM_H

#include <cairo.h>

struct carbon_term;
typedef struct carbon_term carbon_term_t;

carbon_term_t *carbon_new_term(cairo_surface_t *surface, const char *font, unsigned size);
void carbon_term_destroy(carbon_term_t *term);

void carbon_term_print(carbon_term_t *term, const char *msg);

#endif
