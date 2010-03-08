#include "buffer.h"

#include <stdio.h>

void termbuf_putchar(void *vb, char c)
{
	termbuf_t *b = vb;
	printf("%c", c);
	/* TODO */
}

void termbuf_puttab(void *vb)
{
	termbuf_t *b = vb;
	printf("%c", '\t');
	/* TODO */
}

void termbuf_putcr(void *vb)
{
	termbuf_t *b = vb;
	printf("%c(CR)", '\r');
	/* TODO */
}

void termbuf_putnl(void *vb)
{
	termbuf_t *b = vb;
	printf("%c(NL)", '\n');
	/* TODO */
}

