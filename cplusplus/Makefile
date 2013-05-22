PREFIX ?= /usr/local
LIBDIR ?= $(PREFIX)/lib
INCDIR ?= $(PREFIX)/include

CXX ?= g++
CXXFLAGS ?= -O2
WFLAGS ?= -pedantic-errors -W -Wall -Wpointer-arith -Wcast-qual -Winline \
	-Wcast-align -Wconversion -Wshadow -Wredundant-decls \
	-Wctor-dtor-privacy -Wnon-virtual-dtor -Wreorder -Weffc++ \
	-Wno-non-template-friend -Wold-style-cast \
	-Woverloaded-virtual -Wsign-promo
INCLUDES = -I./

AR ?= ar
LINKFLAGS ?= rcs

RM ?= rm -f
INSTALL ?= install
INSTALL_DATA ?= $(INSTALL) -c -m 444

OBJDIR = .obj
BLDDIR = lib
SRCDIR = src
DOCAPIDIR = doc

HEADERS = \
	src/dynrules.h \
	src/LearnSystem.h \
	src/MMapRuleManager.h \
	src/Rule.h \
	src/RuleManager.h \
	src/RuleSet.h

SOURCES = Rule.cpp  RuleSet.cpp  LearnSystem.cpp  RuleManager.cpp \
	MMapRuleManager.cpp

OBJECTS = $(SOURCES:%.cpp=%.o)
TARGET = libdynrules.a

# Example flags.
EXINCLUDES = -I./ -I./src
LFLAGS = -L$(BLDDIR)/ -ldynrules -lstdc++
EXAMPLES = learnsystem

all: clean dirs $(OBJECTS) $(TARGET)

docs:
	@cd $(DOCAPIDIR)/ && doxygen api.doxygen

dirs:
	@mkdir -p $(OBJDIR) $(BLDDIR)

$(OBJECTS): dirs
	$(CXX) $(CFLAGS) $(WFLAGS) $(INCLUDES) -c $(SRCDIR)/$*.cpp -o $(OBJDIR)/$*.o

$(TARGET): $(OBJECTS)
	$(AR) $(LINKFLAGS) $(BLDDIR)/$(TARGET) $(OBJECTS:%.o=$(OBJDIR)/%.o)

clean:
	$(RM) $(SRCDIR)/*~ examples/*~ $(EXAMPLES)
	$(RM) -r $(OBJDIR) $(BLDDIR) $(DOCAPIDIR)/html

install: $(TARGET)
	$(INSTALL) -d $(INCDIR)/dynrules
	$(INSTALL_DATA) $(HEADERS) $(INCDIR)/dynrules
	$(INSTALL) -d $(LIBDIR)
	$(INSTALL) -s $(BLDDIR)/$(TARGET) $(LIBDIR)/$(TARGET)

# Examples
examples: learnsystem

learnsystem:
	$(CXX) $(CXXFLAGS) -static $(WFLAGS) $(EXINCLUDES) \
		examples/learnsystem.cpp -o learnsystem $(LFLAGS)
