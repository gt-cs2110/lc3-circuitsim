CC = gcc
CFLAGS = -g -std=c99 -pedantic -Wall -Werror -Wextra -Wstrict-prototypes -Wold-style-definition

PROG = genrom
# The C and H files
CFILES = $(wildcard *.c)
HFILES = $(wildcard *.h)
OFILES = $(patsubst %.c,%.o,$(CFILES))

.PHONY: clean

$(PROG): $(OFILES)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c $(HFILES)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(PROG) $(OFILES)
