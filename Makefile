.POSIX:

CONFIGFILE = config.mk
include $(CONFIGFILE)

all: learn-your-telephone-number

learn-your-telephone-number: learn-your-telephone-number.o
	$(CC) -o $@ $@.o $(LDFLAGS)

clean:
	-rm -f -- learn-your-telephone-number *.o

.SUFFIXES:
.SUFFIXES: .o .c

.PHONY: all install uninstall clean
