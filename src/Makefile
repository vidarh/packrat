
OBJS=main.o

APPDIR=../App
MKDIR=mkdir
CP=cp

all: packrat

clean:
	rm packrat
	rm *.o
	rm *~

packrat: $(OBJS) packrat.h
	$(CC) -o packrat $(CFLAGS) $(LDFLAGS) $(OBJS)
	-$(MKDIR) $(APPDIR) 2>/dev/null || true
	$(CP) packrat $(APPDIR)

