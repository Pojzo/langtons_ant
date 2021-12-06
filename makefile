# Makefile

CROSS_COMPILE=
CC=$(CROSS_COMPILE)gcc

SRCDIR=src
SRCEXT=c
BUILDDIR=build
BINDIR=bin
BINNAME=main

BIN=$(BINDIR)/$(BINNAME)
SOURCES=$(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJECTS=$(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SOURCES:.$(SRCEXT)=.o))

CFLAGS=-g -Wall -Werror -Wextra -pedantic -I./include -L./lib -lSDL2
LIB=-lSDL2 -lm

$(BIN): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $^ -o $(BIN) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	$(RM) -r $(BUILDDIR) $(BINDIR)

run: 
	./bin/main
