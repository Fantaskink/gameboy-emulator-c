IDIR =./include
CC=gcc
CFLAGS =-I$(IDIR) -O2 -Wall -fsanitize=address

ODIR=obj
LDIR=lib

_DEPS = cpu.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o cpu.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: src/%.c $(DEPS)
				$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
				$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
				rm -f $(ODIR)/*.o *~ core $(IDIR)/*~ 