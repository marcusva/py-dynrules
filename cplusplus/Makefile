CC ?= g++
CFLAGS ?= -g -O0
WFLAGS ?= -pedantic-errors -W -Wall -Wpointer-arith -Wcast-qual -Winline \
	-Wcast-align -Wconversion -Wshadow -Wredundant-decls \
	-Wctor-dtor-privacy  -Wnon-virtual-dtor -Wreorder -Weffc++ \
	-Wstrict-null-sentinel  -Wno-non-template-friend -Wold-style-cast \
	-Woverloaded-virtual -Wsign-promo 
RM ?= rm -f
AR ?= ar
LINKFLAGS ?= rcs

SOURCES = src/Rule.cpp src/RuleSet.cpp src/LearnSystem.cpp
OBJECTS = $(SOURCES:%.cpp=%.o)
INCLUDES = -I./
TARGET = libdynrules.a

EXINCLUDES = -I./ -I./src
LFLAGS = -L./ -ldynrules -lstdc++
EXAMPLES = learnsystem


all: clean $(OBJECTS) link

.cpp.o:
	$(CC) $(CFLAGS) $(WFLAGS) $(INCLUDES) -c -o $@ $<

link:
	$(AR) $(LINKFLAGS) $(TARGET) $(OBJECTS)

clean:
	$(RM) *.o *.a *~ src/*.o src/*~ examples/*~ $(EXAMPLES)

# Examples
examples: learnsystem

learnsystem:
	$(CC) $(CFLAGS) -static $(WFLAGS) $(EXINCLUDES) \
		examples/learnsystem.cpp -o learnsystem $(LFLAGS) 
