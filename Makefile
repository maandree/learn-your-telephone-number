.POSIX:

CONFIGFILE = config.mk
include $(CONFIGFILE)


all: learn-your-telephone-number

.c.o:
	$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

.o:
	$(CC) -o $@ $< $(LDFLAGS)

install: learn-your-telephone-number
	mkdir -p -- "$(DESTDIR)$(PREFIX)/bin/"
	mkdir -p -- "$(DESTDIR)$(MANPREFIX)/man1/"
	cp -- learn-your-telephone-number "$(DESTDIR)$(PREFIX)/bin/"
	cp -- learn-your-telephone-number.1 "$(DESTDIR)$(MANPREFIX)/man1/"

uninstall:
	-rm -f -- "$(DESTDIR)$(PREFIX)/bin/learn-your-telephone-number"
	-rm -f -- "$(DESTDIR)$(MANPREFIX)/man1/learn-your-telephone-number.1"

clean:
	-rm -f -- learn-your-telephone-number *.o *.su

.SUFFIXES:
.SUFFIXES: .o .c

.PHONY: all install uninstall clean
