#include "buffer.h"

#include <stdio.h>

void termbuf_putchar(void *vb, char c)
{
	termbuf_t *b = vb;
	printf("%c", c);
	fflush(stdout);
	/* TODO */
}

void termbuf_puttab(void *vb)
{
	termbuf_t *b = vb;
	printf("%c", '\t');
	fflush(stdout);
	/* TODO */
}

void termbuf_putcr(void *vb)
{
	termbuf_t *b = vb;
	printf("%c(CR)", '\r');
	fflush(stdout);
	/* TODO */
}

void termbuf_putnl(void *vb)
{
	termbuf_t *b = vb;
	printf("%c(NL)", '\n');
	fflush(stdout);
	/* TODO */
}

