IDIR =.
JSONDIR=../dependencies/json-c
UNITDIR=../dependencies/minunit
CC=gcc
CFLAGS=-I$(IDIR) -I$(JSONDIR) -I$(UNITDIR) -fPIC

ODIR=obj
LDIR =../lib

LIBS=-lm -lrt -ljson

PDIR=Debug
TESTDIR=tests

_DEPS = card.h combo.h hand.h range.h style.h ll.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = card.o combo.o hand.o range.o style.o ll.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

CURRENT_DIR = $(shell pwd)
LD_LIBRARY_PATH = $(CURRENT_DIR)/$(PDIR)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(PDIR)/libtexanal.so: $(OBJ)
	$(CC) -shared -L$(LDIR) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: test
.PHONY: clean

test: $(PDIR)/libtexanal.so
	export LD_LIBRARY_PATH
	$(CC) -L$(LDIR) -L../texanal/$(PDIR) -o $(PDIR)/$@ $(TESTDIR)/$@.c $(CFLAGS) $(LIBS) -ltexanal
	./$(PDIR)/$@
	rm -rf $(PDIR)/$@

clean:
	rm -f $(ODIR)/*.o $(PDIR)/*
