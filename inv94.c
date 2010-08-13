#include "common.c"

void process (struct iop *io) {
	while (1) {
		int d;
		int c = GETC (io);
		if (c == EOF) break;
		if (c >= '!' && c <= '~') {
			c = ('!' + '~') - c;
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
