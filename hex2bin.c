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
		int c = GETC (io);
		if (c == EOF) break;
		c = hexof (c);
		if (c >= 0) {
			int d = GETC (io);
			if (d == EOF) {
				putchar (c);
				break;
			}
			d = hexof (d);
			if (d >= 0) {
				putchar (16 * c + d);
			} else {
				putchar (c);
			}
		}
	}
}
