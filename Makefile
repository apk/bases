BINDIR=bin

all : $(BINDIR)/e64

$(BINDIR)/e64 : e64.c
	mkdir -p $(BINDIR)
	gcc -o $(BINDIR)/e64 e64.c
