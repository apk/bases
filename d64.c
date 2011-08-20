#define INIT

#define OPT_LAZY

#include "common.c"

#include "code64.c"

static int chartable [256];

static void init () {
	int i;
	for (i = 0; i < 256; i ++) chartable [i] = -1;
	for (i = 0; i < sizeof (b64code) / sizeof (b64code [0]); i ++) {
		i [b64code] [chartable] = i;
	}
	chartable ['='] = -2;
}

void process (struct iop *io) {
	while (1) {
		int c;
		int h = GETC (io);
		if (h == EOF) break;
		if (h == ' ' || h == '\n') continue;
		c = chartable [h & 255];
		if (c >= 0) {
			int d;
			h = GETC (io);
			if (h == EOF) {
				// XXX Ugly exit; is bad data
				break;
			}
			d = chartable [h & 255];
			if (d >= 0) {
				PUTC (io, (c << 2) + (d >> 4));
				h = GETC (io);
				if (h == EOF) {
					// XXX Ugly exit; is bad data
					break;
				}
				c = chartable [h & 255];
				if (c >= 0) {
					PUTC (io, (d << 4) + (c >> 2));
					h = GETC (io);
					if (h == EOF) {
						// XXX Ugly exit; is bad data
						break;
					}
					d = chartable [h & 255];
					if (d >= 0) {
						PUTC (io, (c << 6) + d);
						continue;
					} else if (d == -2) {
						continue;
					}
				} else if (c == -2) {
					continue;
				}
			} else if (d == -2) {
				continue;
			}
		} else if (c == -2) {
			continue;
		}
		/* Catch char in h and process */
		if (!lazy) {
			fprintf (stderr, "Bad char in input '%s'\n",
				 io->ifn);
			exit (1);
		}
	}
}
