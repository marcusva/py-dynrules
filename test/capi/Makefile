SOURCES = \
	test_crule.c \
	test_cruleset.c

OBJECTS = $(SOURCES:%.c=%.o)
TARGETS = test_crule test_cruleset

CC = gcc
CFLAGS = -W -Wall -g 

# Windows includes (MinGW)
#INCLUDES = -DWIN32 -I. -Ic:\\Python26\\include
#LIBS = -Lc:\\Python26\\libs -mwindows -lm -lpython26

# Linux/Unix includes
INCLUDES = -I. -I/usr/local/include/ -I/usr/local/include/python3.2mu
LIBS = -L/usr/local/lib -lpython3.2mu -lm

CFLAGS += $(INCLUDES)
LDFLAGS += $(LIBS)

all: $(OBJECTS) $(TARGETS)

test_crule.o: test_crule.c
	$(CC) $(CFLAGS) -c -o $@ $*.c
test_crule: test_crule.o
	$(CC) test_crule.o -o test_crule $(LDFLAGS) 

test_cruleset.o: test_cruleset.c
	$(CC) $(CFLAGS) -c -o $@ $*.c
test_cruleset: test_cruleset.o
	$(CC) test_cruleset.o -o test_cruleset $(LDFLAGS) 

clean:
	rm -f *.o *.core *~ $(TARGETS)
