#include "common.c"

#include "code64.c"

static void out (struct iop *io, int c) {
	PUTC (io, b64code [c & 63]);
}

void process (struct iop *io) {
	int cnt = 0;
	/* XXX if(debug)? fprintf (stderr, "Doing %s\n", io->ifn); */
	/*
	 * XXX Linebreaks at other than 64; at newlines; other separators,
	 * XXX no padding...
	 */
	while (1) {
		int d;
		int c = GETC (io);
		if (c == EOF) break;
		cnt ++;
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
		if (cnt == 16) {
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
