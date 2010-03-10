#include "tty.h"

#define _XOPEN_SOURCE 600
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>

#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "util.h"

#define ESCSIZ 256

#define CSI '['

typedef struct esc_t {
	char pre;
	char mode;
	int args[16];        /* TODO: set to something sane */

	int narg;
} esc_t;

static esc_t *esc_create();
static void esc_clear(esc_t *);
static int esc_grab(esc_t *, char c);

static esc_t *esc_create()
{
	esc_t *e = malloc(sizeof(esc_t));
	esc_clear(e);
	return e;
}

/* reset */
static void esc_clear(esc_t *e)
{
	/* TODO: just memset everything now to zero */
	e->pre = 0;
	e->narg = 0;
	memset(e->args, 0, sizeof(e->args));
}

/* parse an escape seqeuence one letter at a time */
static int esc_grabCSI(esc_t *e, char c)
{
	if (isdigit(c)) {
		e->args[e->narg] *= 10;
		e->args[e->narg] += c - '0';
		return 1;
	}
	else if (c == ';') {
		++e->narg;
		return 1;
	}
	else if (0x40 <= c && c <= 0x7E) {
		e->mode = c;
		return 0;
	}
	/* TODO: error reporting */
	return 0;
}

static int esc_grab(esc_t *e, char c)
{
	if (e->pre == 0) {
		switch (c) {
			case CSI:
				e->pre = c;
				return 1;
			default:
				return 0;
		}
	}
	else switch (e->pre) {
		case CSI:
			return esc_grabCSI(e, c);
	}
	die("wtf?\n");
	return 0;
}

/* apply the escape sequence's effect */
static void esc_applyCSI(esc_t *e, const buf_t *bufimpl, void *buf)
{
	switch(e->mode) {
		/* insert [0] blank characters */
		case '@':
			if (bufimpl->insertblank)
				bufimpl->insertblank(buf, e->args[0] ? e->args[0] : 1);
			break;
		/* move cursor up [0] */
		case 'A':
		case 'e':
			break;
		/* move cursor down [0] */
		case 'B':
			break;
		/* move cursor right [0] */
		case 'C':
		case 'a':
			break;
		/* move cursor left [0] */
		case 'D':
			break;

		/* move cursor down [0] and to start of line */
		case 'E':
			break;
		/* move cursor up [0] and to start of line */
		case 'F':
			break;

		/* more to column [0] */
		case 'G':
		case '`':
			break;
		/* move to row [0] column [1] */
		case 'H':
		case 'f':
			break;

		/* clear screen */
		case 'J':
			break;
		/* clear line */
		case 'K':
			break;

		/* insert [0] blank lines */
		case 'L':
			break;
		/* ??? */

		case 'l':
			break;
		/* delete [0] lines */
		case 'M':
			break;
		/* delete [0] charactes */
		case 'P':
			break;

		/* move to row [0] */
		case 'd':
			break;

		/* set terminal mode */
		case 'h':
			break;
		/* set terminal attributes [] */
		case 'm':
			break;

		/* ??? */
		case 'r':
			break;

		/* save cursor position */
		case 's':
			break;
		/* load cursor position */
		case 'u':
			break;
		default:
			break;
	}
}

static void esc_apply(esc_t *e, const buf_t *bufimpl, void *buf)
{
	switch (e->pre) {
		case CSI:
			esc_applyCSI(e, bufimpl, buf);
			break;
		default:
			fprintf(stderr, "Could not understand escape sequence\n");
	}
}

struct tty_t {
	int   fd;
	pid_t pid;

	int rows, cols;

	int in_esc;
	esc_t *esc;

	const buf_t *bufimpl;
	void *buf;
};

static void tty_new(tty_t *t, shellfn shell)
{
	int masterfd, slavefd;
	char *slavedev;
	pid_t pid;
	
	if ((masterfd = posix_openpt(O_RDWR | O_NOCTTY)) == -1
		|| grantpt(masterfd) == -1
		|| unlockpt(masterfd) == -1
		|| (slavedev = ptsname(masterfd)) == NULL)
		die("failed to create pts\n");

	slavefd = open(slavedev, O_RDWR | O_NOCTTY);
	if (slavefd < 0)
		die("failed to open pts\n");

	switch (pid = fork()) {
		case -1:
			die("fork failed\n");
			break;
		case 0:
			setsid();
			dup2(slavefd, STDIN_FILENO);
			dup2(slavefd, STDOUT_FILENO);
			dup2(slavefd, STDERR_FILENO);
			if (ioctl(slavefd, TIOCSCTTY, NULL) < 0)
				die("ioctl failed\n");
			shell();
			break;
		default:
			close(slavefd);
			t->pid = pid;
			t->fd = masterfd;
	}
}

tty_t *tty_create(int rows, int cols, shellfn shell, const buf_t *bufimpl, void *buf)
{
	tty_t *t = malloc(sizeof(tty_t));

	t->bufimpl = bufimpl;
	t->buf = buf;
	t->rows = rows;
	t->cols = cols;

	t->esc = esc_create();
	t->in_esc = 0;

	/* create a new tty and start the shell on it */
	tty_new(t, shell);
	return t;
}

void tty_destroy(tty_t *t)
{
	free(t);
}

void tty_resize(tty_t *t, int rows, int cols)
{
	t->rows = rows;
	t->cols = cols;
	struct winsize size = {
		.ws_col = cols,
		.ws_row = rows,
		.ws_xpixel = 0,
		.ws_ypixel = 0,
	};
	if (ioctl(t->fd, TIOCSWINSZ, &size) < 0)
		die("ioctl failed, couldn't set window size\n");
}

static void tty_put(tty_t *t, char *buf, int len)
{
	char c;
	for(; len > 0; len--) {
		c = *(buf++);
		if (c == 033) {
			esc_clear(t->esc);
			t->in_esc = 1;
		}
		else if (t->in_esc) {
			t->in_esc = esc_grab(t->esc, c);
			if (!t->in_esc)
				esc_apply(t->esc, t->bufimpl, t->buf);
		}
		else switch(c) {
			/* handle tab */
			case '\t':
				t->bufimpl->puttab(t->buf);
				break;
			/* handle backspace */
			case '\b':
				/* TODO */
				break;
			/* handle carriage return */
			case '\r':
				t->bufimpl->putcr(t->buf);
				break;
			/* handle new line */
			case '\n':
				t->bufimpl->putnl(t->buf);
				break;
			/* handle bell */
			case '\a':
				/* TODO */
				break;
			/* handle other */
			default:
				t->bufimpl->putchar(t->buf, c);
				break;
		}
	}
}

void tty_send(tty_t *t, const char *str, int len)
{
	if(write(t->fd, str, len) == -1)
		die("write error on tty\n");
}

void tty_read(tty_t *t)
{
	char buf[BUFSIZ];
	int ret;

	ret = read(t->fd, buf, BUFSIZ);
	if(ret == -1)
		die("Couldn't read from shell\n");
	tty_put(t, buf, ret);
}

int tty_getfd(tty_t *t)
{
	return t->fd;
}

