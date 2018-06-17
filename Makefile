# dwm-sm - dwm status manager
# See LICENSE file for copyright and license details.

include config.mk

SRC = dwm-sm.c
OBJ = ${SRC:.c=.o}

all: options dwm-sm

options:
	@echo dwm-sm build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.h config.mk

config.h:
	cp config.def.h $@

dwm-st: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f dwm-sm ${OBJ} dwm-sm-${VERSION}.tar.gz

dist: clean
	mkdir -p dwm-sm-${VERSION}
	cp -R LICENSE Makefile README config.def.h config.mk ${SRC}\
		dwm-sm-${VERSION}
	tar -cf dwm-sm-${VERSION}.tar dwm-sm-${VERSION}
	gzip dwm-sm-${VERSION}.tar
	rm -rf dwm-sm-${VERSION}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f dwm-sm ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/dwm-sm

uninstall:
	rm -f $(dst)/usr/bin/$(out)
