#ifndef CARBON_UTIL
#define CARBON_UTIL

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#define STRERR            (strerror(errno))
#define LEN(v)            (sizeof(v) / sizeof((v)[0]))

#define MAX(a, b)         ((a) > (b) ? (a) : (b))
#define MIN(a, b)         ((a) < (b) ? (a) : (b))
#define DEFAULT(v, a)     ((v) = ((v) ? (v) : (a)))
#define LIMIT(v, a, b)    ((v) = ((v) < (a) ? (a) : ((v) > (b) ? (b) : (v))))

void die(const char *err, ...);

#endif
