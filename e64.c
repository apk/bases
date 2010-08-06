#include <stdio.h>

struct iop {
	char buf [1 < 14];
	int pos;
	int len;
};

#define GETC(io) getchar()
#define PUTC(io,c) putchar(c)

static void out (struct iop *io, int c) {
	c &= 63;
	if (c < 26) {
		PUTC (io, 'A' + c);
	} else if (c < 52) {
		PUTC (io, 'a' - 26 + c);
	} else if (c < 62) {
		PUTC (io, '0' - 52 + c);
	} else if (c < 63) {
		PUTC (io, '+');
	} else {
		PUTC (io, '/');
	}
}

void process (struct iop *io) {
	while (1) {
		int d;
		int c = GETC (io);
		if (c == EOF) break;
		out (io, c >> 2);
		c <<= 4;
		d = GETC (io);
		if (d == EOF) {
			out (io, c);
			PUTC (io, '=');
			PUTC (io, '=');
			break;
		}
		out (io, c | (15 & (d >> 4)));
		d <<= 2;
		c = GETC (io);
		if (c == EOF) {
			out (io, d);
			PUTC (io, '=');
			break;
		}
		out (io, d | (3 & (c >> 6)));
		out (io, c);
	}
	PUTC (io, '\n');
}

int main (int argc, char **argv) {
	process (0);
	return 0;
}

/*
 * Local Variables:
 * compile-command: "sh dotests"
 * End:
 */
