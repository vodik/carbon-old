#define _XOPEN_SOURCE 600
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <xcb/xcb.h>

#include "tty.h"
#include "util.h"
#include "buffer.h"

#define TERM "carbon-256color"
static const char fontname[] = "-*-terminus-medium-r-normal-*-12-*-*-*-*-*-*-*";

xcb_connection_t *conn;
tty_t *tty;

static void expose(xcb_generic_event_t *event);
static void keypress(xcb_generic_event_t *event);

//static void (*handler[LASTEvent])(xcb_generic_event_t *) = {
static void (*handler[14])(xcb_generic_event_t *) = {
	[XCB_EXPOSE] = expose,
	[XCB_KEY_RELEASE] = keypress,
};

static void expose(xcb_generic_event_t *event)
{
}

static void keypress(xcb_generic_event_t *e)
{
	xcb_key_release_event_t *event = (xcb_key_release_event_t *)e;
	//printf("keypress\n");a
	//char c = (char)event->detail;
	//printf("send: %d\n", event->detail.key);
	//printf("send: %c\n", c);
	//tty_send(tty, c);
}

/*static const buf_t buffer = {
	.putc_ = mybuf_putc;
	.puttab = mybuf_puttab;
	.putcr = mybuf_putcr;
	.putnl = mybuf_putnl;
}*/

static void diecookie(xcb_void_cookie_t cookie, const char *err, ...)
{
	va_list vargs;
	xcb_generic_error_t *error = xcb_request_check(conn, cookie);

	if (error) {
		xcb_disconnect(conn);
		va_start(vargs, err);
		vfprintf(stderr, err, vargs);
		va_end(vargs);

		exit(EXIT_FAILURE);
	}
}

static void xinit()
{
	int screen_no;
	xcb_screen_iterator_t iter;
	xcb_window_t window;
	xcb_void_cookie_t cookie;
	int i;

	conn= xcb_connect(NULL, &screen_no);
	if (!conn)
		die("couldn't connect to the X server\n");

	iter = xcb_setup_roots_iterator(xcb_get_setup(conn));
	for (i = 0; i < screen_no; ++i)
		xcb_screen_next(&iter);

	xcb_screen_t *screen = iter.data;
	if (!screen) {
		xcb_disconnect(conn);
		die("couldn't get the current screen\n");
	}

	xcb_font_t font = xcb_generate_id(conn);
	cookie = xcb_open_font_checked(conn,
			font,
			strlen(fontname),
			fontname);
	diecookie(cookie, "couldn't open font\n");

	window = xcb_generate_id(conn);

	uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	uint32_t values[2];
	values[0] = screen->white_pixel;
	values[1] = XCB_EVENT_MASK_KEY_RELEASE |
	            XCB_EVENT_MASK_BUTTON_PRESS |
	            XCB_EVENT_MASK_POINTER_MOTION |
	            XCB_EVENT_MASK_EXPOSURE;

	cookie = xcb_create_window_checked(conn,
			screen->root_depth,
			window,
			screen->root,
			20, 200,
			400, 400,
			0,
			XCB_WINDOW_CLASS_INPUT_OUTPUT,
			screen->root_visual,
			mask, values);
	diecookie(cookie, "can't create window\n");
	cookie = xcb_map_window_checked(conn, window);
	diecookie(cookie, "couldn't map window\n");

	xcb_flush(conn);
}

static void run()
{
	int xcbfd = xcb_get_file_descriptor(conn);
	int ttyfd = tty_getfd(tty);
	xcb_generic_event_t *event;
	fd_set rfd;

	while (1) {
		FD_ZERO(&rfd);
		FD_SET(xcbfd, &rfd);
		FD_SET(ttyfd, &rfd);

		/* wait until there are either x events to process
		 * or input to draw from our tty */
		if (select(MAX(xcbfd, ttyfd) + 1, &rfd, NULL, NULL, NULL) == -1)
			die("select failed\n");

		/* if there is input from our tty, draw it */
		if (FD_ISSET(ttyfd, &rfd)) {
			tty_read(tty);
			/* TODO: render out */
		}

		/* always check for xcb events. some may have arived durring
		 * drawing */
		event = xcb_poll_for_event(conn);
		if (event) {
			if (handler[event->response_type])
				handler[event->response_type](event);
			free(event);
		}
	}
}

void shell()
{
	char *args[] = { getenv("SHELL"), "-i", NULL };
	putenv("TERM=" TERM);
	execv(args[0], args);
}

int main(int argc, char *argv[])
{
	xinit();
	tty = tty_create(80, 24, shell, &termbuf_impl, NULL);

	run();
}
