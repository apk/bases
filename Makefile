# Generated by dotests...
BINDIR=.bin
all ::

all :: $(BINDIR)/e64

$(BINDIR)/e64 : e64.c common.c
	mkdir -p $(BINDIR)
	gcc -o $(BINDIR)/e64 e64.c

all :: $(BINDIR)/rot13

$(BINDIR)/rot13 : rot13.c common.c
	mkdir -p $(BINDIR)
	gcc -o $(BINDIR)/rot13 rot13.c

all :: $(BINDIR)/hex2bin

$(BINDIR)/hex2bin : hex2bin.c common.c
	mkdir -p $(BINDIR)
	gcc -o $(BINDIR)/hex2bin hex2bin.c

all :: $(BINDIR)/bin2hex

$(BINDIR)/bin2hex : bin2hex.c common.c
	mkdir -p $(BINDIR)
	gcc -o $(BINDIR)/bin2hex bin2hex.c

all :: $(BINDIR)/inv94

$(BINDIR)/inv94 : inv94.c common.c
	mkdir -p $(BINDIR)
	gcc -o $(BINDIR)/inv94 inv94.c
