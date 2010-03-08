#include "util.h"

#include <stdlib.h>
#include <stdio.h>

void die(const char *err, ...)
{
	va_list vargs;

	va_start(vargs, err);
	vfprintf(stderr, err, vargs);
	va_end(vargs);

	exit(EXIT_FAILURE);
}
