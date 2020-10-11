CC = gcc

ifndef O
	O = 3
endif

CFLAGS = -g -O$O -Wall $(shell pkg-config --cflags x11 xscrnsaver)
LDFLAGS += $(shell pkg-config --libs x11 xscrnsaver)
VPATH = src
OBJECTS = main

.PHONY: all clean install uninstall
all: clean $(OBJECTS) keyfault
main: main.c; $(CC) -c $(CFLAGS) $^ -o $(VPATH)/$@.o
keyfault: $(foreach object, $(OBJECTS), $(VPATH)/$(object).o); $(CC) $(CFLAGS) -o data/usr/bin/$@ $^ $(LDFLAGS)

clean:

	$(shell rm -f ./data/usr/bin/keyfault)
	$(shell rm -f $(foreach librarydir, $(VPATH), $(librarydir)/*.o))

install:

	$(shell install -d $(DESTDIR)/usr/bin)
	$(shell install ./data/usr/bin/keyfault $(DESTDIR)/usr/bin)
	$(shell install -d $(DESTDIR)/etc/xdg/autostart)
	$(shell install ./data/etc/xdg/autostart/keyfault.desktop $(DESTDIR)/etc/xdg/autostart)

uninstall:

	$(shell rm -f $(DESTDIR)/usr/bin/keyfault)
	$(shell rm -f $(DESTDIR)/etc/xdg/autostart/keyfault.desktop)

