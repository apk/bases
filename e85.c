#include "common.c"

#include "code85.c"

static void out (struct iop *io, int n, unsigned int c) {
	if (n > 0) {
		out (io, n - 1, c / 85);
		PUTC (io, b85code [c % 85]);
	}
}

void process (struct iop *io) {
	int cnt = 0;
	while (1) {
		unsigned int c;
		int n = GETC (io);
		if (n == EOF) break;
		cnt ++;
		c = n & 255;
		for (n = 0; n < 3; n ++) {
			int d = GETC (io);
			if (n == EOF) break;
			c = (c << 8) + (d & 255);
		}
		out (io, n + 2, c);
		if (cnt == 14) {
			PUTC (io, '\n');
			cnt = 0;
		}
	}
	if (cnt > 0) {
		PUTC (io, '\n');
	}
}

/*
 * Local Variables:
 * compile-command: "sh dotests"
 * End:
 */
