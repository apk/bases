#include "common.c"

void process (struct iop *io) {
	while (1) {
		int d;
		int c = GETC (io);
		if (c == EOF) break;
		d = c & ~' ';
		if (d >= 'A' && d <= 'M') {
			c += 13;
		} else if (d >= 'N' && d <= 'Z') {
			c -= 13;
		}
		PUTC (io, c);
	}
	if (io->fd == -1) {
		PUTC (io, '\n');
	}
}

/*
 * Local Variables:
 * compile-command: "sh dotests"
 * End:
 */
