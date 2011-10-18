#include "common.c"

void process (struct iop *io) {
	while (1) {
		int c = GETC (io);
		if (c == EOF) break;
		usleep (3000);
		PUTC (io, c);
		FLUSH (io);
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
