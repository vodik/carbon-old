/*
 * key.h - Keybinding helpers
 *
 * Copyright © 2009 Julien Danjou <julien@danjou.info>
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

#ifndef AWESOME_KEY_H
#define AWESOME_KEY_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>

int key_press_lookup_string(xcb_keysym_t, char *, ssize_t);
xcb_keysym_t key_getkeysym(xcb_key_symbols_t *keysyms, xcb_keycode_t detail, uint16_t state);

#endif

// vim: filetype=c:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:encoding=utf-8:textwidth=80
