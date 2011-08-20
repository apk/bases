#include "common.c"

void process (struct iop *io) {
	int c;
	int n = 0;
	char *beg = "\t";
	while ((c = GETC (io)) != EOF) {
		char buf [40];
		if (n == 8) {
			PUTS (io, ",\n");
			n = 0;
			beg = "\t";
		}
		sprintf (buf, "%s0x%02x", beg, c & 255);
		PUTS (io, buf);
		beg = ", ";
		n ++;
	}
	if (n > 0) {
		PUTC (io, '\n');
	}
}

/*
 * Local Variables:
 * compile-command: "sh dotests"
 * End:
 */
