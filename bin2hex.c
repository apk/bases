#define OPTS

#include "common.c"

static char hex [] = "0123456789abcdef";

static int lim = 36;

void process (struct iop *io) {
	int z = 0;
	while (1) {
		int c = GETC (io);
		if (c == EOF) break;
		PUTC (io, hex [(c >> 4) & 15]);
		PUTC (io, hex [c & 15]);
		z ++;
		if (z == lim) {
			PUTC (io, '\n');
			z = 0;
		}
	}
	if (z > 0) {
		PUTC (io, '\n');
	}
}

static int opts (char *opt) {
	int l = atoi (opt);
	if (l > 0) {
		lim = l;
		return 1;
	}
	if (!strcmp (opt, "z")) {
		lim = 0;
		return 1;
	}
	return 0;
}
