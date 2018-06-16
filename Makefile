cc = gcc
src = dwm-sm.c
out = dwm-sm

cflags = -std=c99 -pedantic -Wall -Wextra -O3 -march=native

ldflags =

dst=/

all: $(src)
	$(cc) $(cflags) $(ldflags) $(src) -o $(out)
install:
	install -m 755 $(out) $(dst)/usr/bin/
uninstall:
	rm -f $(dst)/usr/bin/$(out)
clean:
	rm -f $(out)
