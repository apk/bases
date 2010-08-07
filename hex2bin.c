#define OPT_LAZY

#include "common.c"

int hexof (int c) {
	switch (c) {
	case '0': return 0;
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	case 'a': case 'A': return 10;
	case 'b': case 'B': return 11;
	case 'c': case 'C': return 12;
	case 'd': case 'D': return 13;
	case 'e': case 'E': return 14;
	case 'f': case 'F': return 15;
	}
	return -1;
}

void process (struct iop *io) {
	while (1) {
		int c;
		int h = GETC (io);
		if (h == EOF) break;
		c = hexof (h);
		if (c >= 0) {
			int d;
			h = GETC (io);
			if (h == EOF) {
				PUTC (io, c);
				break;
			}
			d = hexof (h);
			if (d >= 0) {
				PUTC (io, 16 * c + d);
				continue;
			} else {
				PUTC (io, c);
			}
		}
		/* Catch char in h and process */
		if (h == ' ' || h == '\n') continue;
		if (!lazy) {
			fprintf (stderr, "Bad char in input '%s'\n",
				 io->ifn);
			exit (1);
		}
		PUTC (io, h);
	}
}
