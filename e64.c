#include <stdio.h>
#include <stdlib.h>

struct iop {
	int fd;
	char *ifn;
	char *ibuf;
	char *iend;
	char ibuffer [1 << 14];
};

#define GETC(io) ((io) ? ((io)->ibuf < (io)->iend		      \
			  ? *(io)->ibuf ++ : readbuf (io))	      \
		  : getchar ())
#define PUTC(io,c) putchar(c)

int readbuf (struct iop *io) {
	int r;
	if (io->ibuf <= io->iend) { /* (Mostly) redundant */
		if (io->fd == -1) return EOF; /* For strings */
		r = read (io->fd, io->ibuffer, sizeof (io->ibuffer));
		if (r < 0) {
			perror ("read"); /* XXX */
			exit (1);
		}
		if (r == 0) {
			return EOF;
		}
		io->ibuf = io->ibuffer;
		io->iend = io->ibuffer + r;
	}
	return *io->ibuf ++;
}

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

int main (int argc, char **argv) {
	static char dummy [] = "<stdin>";
	int donesome = 0;
	int donestdin = 0;
	int i;
	for (i = 1; i < argc; i ++) {
		char *arg = argv [i];
		if (arg [0] != '-') {
			struct iop IO;
			int fd = open (arg, 0);
			if (fd == -1) {
				fprintf (stderr, "Can't open %s\n", arg);
				exit (1);
			}
			IO.ibuf = dummy;
			IO.iend = dummy;
			IO.fd = fd;
			IO.ifn = arg;
			process (&IO);
			donesome = 1;
			continue;
		}
		if (arg [1] == 0) {
			/* stdin */
			struct iop IO = { 0, dummy, dummy, dummy };
			if (donestdin) {
				fprintf (stderr, "Can do stdin once only\n");
				exit (1);
			}
			process (&IO);
			donesome = 1;
			donestdin = 1;
			continue;
			exit (1);
		}
		if (arg [1] == 's') {
			/* String */
			struct iop IO;
			IO.ibuf = (arg += 2);
			IO.ifn = arg;
			IO.fd = -1;
			while (*arg) arg ++;
			IO.iend = arg;
			process (&IO);
			donesome = 1;
			continue;
		}
		fprintf (stderr, "Bad option %s\n", arg);
		exit (1);
	}
	if (!donesome) {
		struct iop IO = { 0, dummy, dummy, dummy };
		process (&IO);
	}
	return 0;
}

/*
 * Local Variables:
 * compile-command: "sh dotests"
 * End:
 */
