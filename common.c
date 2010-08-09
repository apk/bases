#include <stdio.h>
#include <stdlib.h>

struct iop {
	int fd;
	char *ifn;
	char *ibuf;
	char *iend;
	char ibuffer [1 << 14];
};

int ofd = 1;
char obuffer [1 << 14];
int ofill = 0;

#define GETC(io) ((io) ? ((io)->ibuf < (io)->iend		      \
			  ? *(io)->ibuf ++ : readbuf (io))	      \
		  : getchar ())
#define PUTC(io,c) (ofill < sizeof (obuffer) ? obuffer [ofill ++] = (c) \
		    : writebuf (c))
#define FLUSH(io) (flushbuf ())

void flushbuf () {
	int p = 0;
	while (p < ofill) {
		int r = write (ofd, obuffer + p, ofill - p);
		if (r <= 0) {
			fprintf (stderr, "Output write error\n");
			exit (1);
		}
		p += r;
	}
	ofill = 0;
}

void writebuf (int c) {
	flushbuf ();
	obuffer [ofill ++] = c;
}

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
static char *opts (char *opt);
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
		char *p;
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
			flushbuf ();
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
			flushbuf ();
			donesome = 1;
			donestdin = 1;
			continue;
			exit (1);
		}
		for (p = arg + 1; *p; ) {
			switch (*p ++) {
				char *h;
				struct iop IO;
				int fd;
			case 'f':
				if (i + 1 == argc) {
					fprintf (stderr, "Missing arg for -f\n");
					exit (1);
				}
				/* String */
				h = argv [++ i];
				fd = open (h, 0);
				if (fd == -1) {
					fprintf (stderr, "Can't open %s\n", h);
					exit (1);
				}
				IO.ibuf = dummy;
				IO.iend = dummy;
				IO.fd = fd;
				IO.ifn = arg;
				process (&IO);
				flushbuf ();
				close (fd); /* XXX No check */
				donesome = 1;
				continue;
			case 'o':
				if (i + 1 == argc) {
					fprintf (stderr, "Missing arg for -o\n");
					exit (1);
				}
				/* String */
				h = argv [++ i];
				fd = creat (h, 0666);
				if (fd == -1) {
					fprintf (stderr, "Can't create %s\n", h);
					exit (1);
				}
				if (ofd != 1) close (ofd); /* XXX No check */
				ofd = fd;
				continue;
			case 'c':
				if (i + 1 == argc) {
					fprintf (stderr, "Missing arg for -s\n");
					exit (1);
				}
				/* String */
				h = argv [++ i];
				IO.ibuf = h;
				IO.ifn = "-s";
				IO.fd = -1;
				while (*h) h ++;
				IO.iend = h;
				process (&IO);
				flushbuf ();
				donesome = 1;
				continue;
#ifdef OPT_LAZY
			case 'r':
				lazy ++;
				continue;
#endif
			default:
#ifdef OPTS
				h = opts (p - 1);
				if (h) {
					p = h;
					continue;
				}
#endif
				fprintf (stderr, "Bad option %c in %s\n",
					 p [-1], arg);
				exit (1);
			}
		}
	}
	if (!donesome) {
		struct iop IO = { 0, dummy, dummy, dummy };
		process (&IO);
		flushbuf ();
	}
	if (ofd != 1) close (ofd); /* XXX No check */
	return 0;
}

/*
 * Local Variables:
 * compile-command: "sh dotests"
 * End:
 */
