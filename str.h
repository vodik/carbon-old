/*
 * util.h - useful functions header
 *
 * Copyright © 2007-2008 Julien Danjou <julien@danjou.info>
 * Copyright © 2006 Pierre Habouzit <madcoder@debian.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef AWESOME_COMMON_UTIL_H
#define AWESOME_COMMON_UTIL_H

#define _GNU_SOURCE

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <assert.h>
#include <stdio.h>

#if !(defined (__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__))
#include <alloca.h>
#endif

static inline ssize_t a_strlen(const char *s)
{
    return s ? strlen(s) : 0;
}

static inline ssize_t a_strnlen(const char *s, ssize_t n)
{
    if (s)
    {
        const char *p = memchr(s, '\0', n);
        return p ? p - s : n;
    }
    return 0;
}

ssize_t a_strncpy(char *dst, ssize_t n, const char *src, ssize_t l) __attribute__((nonnull(1)));
ssize_t a_strcpy(char *dst, ssize_t n, const char *src) __attribute__((nonnull(1)));

/** \brief safe strcat.
 *
 * The a_strcat() function appends the string \c src at the end of the buffer
 * \c dst if space is available.
 *
 * \param[in]  dst   destination buffer.
 * \param[in]  n     size of the buffer, Negative sizes are allowed.
 * \param[in]  src   the string to append.
 * \return <tt>a_strlen(dst) + a_strlen(src)</tt>
 */
static inline ssize_t a_strcat(char *dst, ssize_t n, const char *src)
{
    ssize_t dlen = a_strnlen(dst, n - 1);
    return dlen + a_strcpy(dst + dlen, n - dlen, src);
}

#endif
// vim: filetype=c:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:encoding=utf-8:textwidth=80
