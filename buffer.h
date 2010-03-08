#ifndef CARBON_BUFFER_H
#define CARBON_BUFFER_H

#include "tty.h"

typedef struct termbuf_t termbuf_t;

void termbuf_putchar(void *, char c);
void termbuf_puttab(void *);
void termbuf_putcr(void *);
void termbuf_putnl(void *);

static const buf_t termbuf_impl = {
	.putchar = termbuf_putchar,
	.puttab = termbuf_puttab,
	.putcr = termbuf_putcr,
	.putnl = termbuf_putnl,
};

#endif
