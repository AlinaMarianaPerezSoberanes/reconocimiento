PROYECTO=procesamiento
DESTDIR=/usr/local/bin
CFLAGS=-Wall
LDFLAGS=-lm -pthread
CC=gcc

all: $(PROYECTO)
%.o: %.c
	$(CC) -c $< $(CFLAGS)
$(PROYECTO): $(patsubst %.c,%.o,$(wildcard *.c))
	$(CC) $^ -o $@ $(LDFLAGS)
.PHONY: clean
install: all
	if [ ! -d $(DESTDIR) ]; then \
		sudo mkdir $(DESTDIR); \
	fi; \
	sudo cp $(PROYECTO) $(DESTDIR)
clean: 
	rm -f *.o $(PROYECTO)
uninstall: 
	sudo rm $(DESTDIR)/$(PROYECTO)
