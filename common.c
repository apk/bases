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

#ifdef OPTS
static int opts (char *opt);
#endif

#ifdef OPT_LAZY
static int lazy = 0;
#endif

void process (struct iop *io);

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
#ifdef OPT_LAZY
		if (!strcmp (arg + 1, "r")) {
			lazy = 1;
			continue;
		}
#endif
#ifdef OPTS
		if (opts (arg + 1)) {
			continue;
		}
#endif
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
