.POSIX:

CONFIGFILE = config.mk
include $(CONFIGFILE)

all: learn-your-telephone-number

learn-your-telephone-number: learn-your-telephone-number.o
	$(CC) -o $@ $@.o $(LDFLAGS)

install: learn-your-telephone-number
	mkdir -p -- "$(DESTDIR)$(PREFIX)/bin/"
	mkdir -p -- "$(DESTDIR)$(MANPREFIX)/man1/"
	mkdir -p -- "$(DESTDIR)$(PREFIX)/share/licenses/learn-your-telephone-number/"
	cp -- learn-your-telephone-number "$(DESTDIR)$(PREFIX)/bin/"
	cp -- learn-your-telephone-number.1 "$(DESTDIR)$(MANPREFIX)/man1/"
	cp -- LICENSE "$(DESTDIR)$(PREFIX)/share/licenses/learn-your-telephone-number/"

uninstall:
	-rm -f -- "$(DESTDIR)$(PREFIX)/bin/learn-your-telephone-number"
	-rm -f -- "$(DESTDIR)$(MANPREFIX)/man1/learn-your-telephone-number.1"
	-rm -rf -- "$(DESTDIR)$(PREFIX)/share/licenses/learn-your-telephone-number/"

clean:
	-rm -f -- learn-your-telephone-number *.o

.SUFFIXES:
.SUFFIXES: .o .c

.PHONY: all install uninstall clean
