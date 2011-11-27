#include <stdlib.h>
#include <stdio.h>
#include <cairo.h>
#include "term.h"

const unsigned width  = 240;
const unsigned height = 240;

int
main(int argc, char *argv[])
{
	cairo_surface_t *surface;
	carbon_term_t *term;

	/* prepare drawing area */
	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
	term = carbon_new_term(surface, "Envy Code R", 12);

	/* write to our terminal */
	carbon_term_print(term, "@echo cleaning up...");
	carbon_term_print(term, "\nCFLAGS = -Wall -pedantic -std=gnu99 `pkg-config --cflags cairo`");

	/* write output and clean up */
	cairo_surface_write_to_png(surface, "terminal.png");

	carbon_term_destroy(term);
	cairo_surface_destroy(surface);
	return 0;
}
