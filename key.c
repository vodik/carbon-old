/*
 * key.c - Key bindings configuration management
 *
 * Copyright © 2008-2009 Julien Danjou <julien@danjou.info>
 * Copyright © 2008 Pierre Habouzit <madcoder@debian.org>
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

#include "key.h"
#include <stdint.h>

/* XStringToKeysym() and XKeysymToString */
#include <X11/Xlib.h>

/* XCB doesn't provide keysyms definition */
#include <X11/keysym.h>
#include <X11/XF86keysym.h>
#include <string.h>
#include "str.h"

#include "keysym_unicode.h"

uint16_t numlockmask, shiftlockmask, capslockmask, modeswitchmask;
xcb_key_symbols_t *keysyms;

static int keysym_to_utf8(char *buf, const xcb_keysym_t ksym)
{
	unsigned int ksym_conv;
	int count;

	/* Unicode keysym */
	if ((ksym & 0xff000000) == 0x01000000)
		ksym_conv = ksym & 0x00ffffff;
	else if (ksym > 0 && ksym < 0x100)
		ksym_conv = ksym;
	else if (ksym > 0x1a0 && ksym < 0x200)
		ksym_conv = keysym_to_unicode_1a1_1ff[ksym - 0x1a1];
	else if (ksym > 0x2a0 && ksym < 0x2ff)
		ksym_conv = keysym_to_unicode_2a1_2fe[ksym - 0x2a1];
	else if (ksym > 0x3a1 && ksym < 0x3ff)
		ksym_conv = keysym_to_unicode_3a2_3fe[ksym - 0x3a2];
	else if (ksym > 0x4a0 && ksym < 0x4e0)
		ksym_conv = keysym_to_unicode_4a1_4df[ksym - 0x4a1];
	else if (ksym > 0x589 && ksym < 0x5ff)
		ksym_conv = keysym_to_unicode_590_5fe[ksym - 0x590];
	else if (ksym > 0x67f && ksym < 0x700)
		ksym_conv = keysym_to_unicode_680_6ff[ksym - 0x680];
	else if (ksym > 0x7a0 && ksym < 0x7fa)
		ksym_conv = keysym_to_unicode_7a1_7f9[ksym - 0x7a1];
	else if (ksym > 0x8a3 && ksym < 0x8ff)
		ksym_conv = keysym_to_unicode_8a4_8fe[ksym - 0x8a4];
	else if (ksym > 0x9de && ksym < 0x9f9)
		ksym_conv = keysym_to_unicode_9df_9f8[ksym - 0x9df];
	else if (ksym > 0xaa0 && ksym < 0xaff)
		ksym_conv = keysym_to_unicode_aa1_afe[ksym - 0xaa1];
	else if (ksym > 0xcde && ksym < 0xcfb)
		ksym_conv = keysym_to_unicode_cdf_cfa[ksym - 0xcdf];
	else if (ksym > 0xda0 && ksym < 0xdfa)
		ksym_conv = keysym_to_unicode_da1_df9[ksym - 0xda1];
	else if (ksym > 0xe9f && ksym < 0xf00)
		ksym_conv = keysym_to_unicode_ea0_eff[ksym - 0xea0];
	else if (ksym > 0x12a0 && ksym < 0x12ff)
		ksym_conv = keysym_to_unicode_12a1_12fe[ksym - 0x12a1];
	else if (ksym > 0x13bb && ksym < 0x13bf)
		ksym_conv = keysym_to_unicode_13bc_13be[ksym - 0x13bc];
	else if (ksym > 0x14a0 && ksym < 0x1500)
		ksym_conv = keysym_to_unicode_14a1_14ff[ksym - 0x14a1];
	else if (ksym > 0x15cf && ksym < 0x15f7)
		ksym_conv = keysym_to_unicode_15d0_15f6[ksym - 0x15d0];
	else if (ksym > 0x169f && ksym < 0x16f7)
		ksym_conv = keysym_to_unicode_16a0_16f6[ksym - 0x16a0];
	else if (ksym > 0x1e9e && ksym < 0x1f00)
		ksym_conv = keysym_to_unicode_1e9f_1eff[ksym - 0x1e9f];
	else if (ksym > 0x209f && ksym < 0x20ad)
		ksym_conv = keysym_to_unicode_20a0_20ac[ksym - 0x20a0];
	else
		return 0;

	count = utf8clen(ksym_conv);
	switch (count) {
		case 7: return 0;
		case 6: buf[5] = (ksym_conv | 0x80) & 0xbf; ksym_conv >>= 6;
		case 5: buf[4] = (ksym_conv | 0x80) & 0xbf; ksym_conv >>= 6;
		case 4: buf[3] = (ksym_conv | 0x80) & 0xbf; ksym_conv >>= 6;
		case 3: buf[2] = (ksym_conv | 0x80) & 0xbf; ksym_conv >>= 6;
		case 2: buf[1] = (ksym_conv | 0x80) & 0xbf; ksym_conv >>= 6;
		case 1: buf[0] = (ksym_conv | __utf8_mark[count]);
	}
	buf[count] = '\0';
	return 1;
}

static int
keysym_to_str(char *buf, ssize_t len, const xcb_keysym_t ksym)
{
    switch (ksym)
    {
		case XK_BackSpace:
			buf[0] = '\b';
			buf[1] = '\0';
			return 1;
		case XK_Tab:
			buf[0] = '\t';
			buf[1] = '\0';
			return 1;
		case XK_Clear:
			break;
		case XK_Return:
			buf[0] = '\n';
			buf[1] = '\0';
			return 1;

		case XK_Left:
			return a_strcpy(buf, len, "\033[D");
		case XK_Up:
			return a_strcpy(buf, len, "\033[A");
		case XK_Right:
			return a_strcpy(buf, len, "\033[C");
		case XK_Down:
			return a_strcpy(buf, len, "\033[B");
		case XK_Delete:
			return a_strcpy(buf, len, "\033[3~");
		case XK_Home:
			return a_strcpy(buf, len, "\033[1~");
		case XK_End:
			return a_strcpy(buf, len, "\033[4~");
		case XK_Prior:
			return a_strcpy(buf, len, "\033[5~");
		case XK_Next:
			return a_strcpy(buf, len, "\033[6~");
		default:
			return 0;
	}
	return 0;
}

int key_press_lookup_string(xcb_keysym_t ksym, char *buf, ssize_t buf_len)
{
	/* Handle special KeySym (Tab, Newline...) */
	if ((ksym & 0xffffff00) == 0xff00)
		return keysym_to_str(buf, buf_len, ksym);
	else
		return keysym_to_utf8(buf, ksym);
}

/** Return the keysym from keycode.
 * \param detail The keycode received.
 * \param state The modifier state.
 * \return A keysym.
 */
xcb_keysym_t key_getkeysym(xcb_key_symbols_t *keysyms, xcb_keycode_t detail, uint16_t state)
{
	xcb_keysym_t k0, k1;

	/* 'col' (third parameter) is used to get the proper KeySym
	 * according to modifier (XCB doesn't provide an equivalent to
	 * XLookupString()).
	 *
	 * If Mode_Switch is ON we look into second group.
	 */
	if (state & modeswitchmask) {
		k0 = xcb_key_symbols_get_keysym(keysyms, detail, 4);
		k1 = xcb_key_symbols_get_keysym(keysyms, detail, 5);
	}
	else {
		k0 = xcb_key_symbols_get_keysym(keysyms, detail, 0);
		k1 = xcb_key_symbols_get_keysym(keysyms, detail, 1);
	}

	/* If the second column does not exists use the first one. */
	if (k1 == XCB_NO_SYMBOL)
		k1 = k0;

	/* The numlock modifier is on and the second KeySym is a keypad
	 * KeySym */
	if ((state & numlockmask) && xcb_is_keypad_key(k1)) {
		/* The Shift modifier is on, or if the Lock modifier is on and
		 * is interpreted as ShiftLock, use the first KeySym */
		if ((state & XCB_MOD_MASK_SHIFT) || (state & XCB_MOD_MASK_LOCK && (state & shiftlockmask)))
			return k0;
		else
			return k1;
	}

	/* The Shift and Lock modifers are both off, use the first
	 * KeySym */
	else if (!(state & XCB_MOD_MASK_SHIFT) && !(state & XCB_MOD_MASK_LOCK))
		return k0;

	/* The Shift modifier is off and the Lock modifier is on and is
	 * interpreted as CapsLock */
	else if (!(state & XCB_MOD_MASK_SHIFT) && (state & XCB_MOD_MASK_LOCK && (state & capslockmask)))
		/* The first Keysym is used but if that KeySym is lowercase
		 * alphabetic, then the corresponding uppercase KeySym is used
		 * instead */
		return k1;

	/* The Shift modifier is on, and the Lock modifier is on and is
	 * interpreted as CapsLock */
	else if ((state & XCB_MOD_MASK_SHIFT) && (state & XCB_MOD_MASK_LOCK && (state & capslockmask)))
		/* The second Keysym is used but if that KeySym is lowercase
		 * alphabetic, then the corresponding uppercase KeySym is used
		 * instead */
		return k1;

	/* The Shift modifier is on, or the Lock modifier is on and is
	 * interpreted as ShiftLock, or both */
	else if ((state & XCB_MOD_MASK_SHIFT) || (state & XCB_MOD_MASK_LOCK && (state & shiftlockmask)))
		return k1;

	return XCB_NO_SYMBOL;
}
