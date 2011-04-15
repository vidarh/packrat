
OBJS=main.o control.o build.o require.o

APPDIR=../App
MKDIR=mkdir
CP=cp

CFLAGS=-g -Wall

ifdef DEBUG
CFLAGS += -DDEBUG
endif

all: packrat

clean:
	rm packrat
	rm *.o
	rm *~

packrat: $(OBJS) packrat.h control.h
	$(CC) -o packrat $(CFLAGS) $(LDFLAGS) $(OBJS)
	-$(MKDIR) $(APPDIR) 2>/dev/null || true
	$(CP) packrat $(APPDIR)

install:
	echo "NOT YET"
