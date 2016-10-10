# Compiler and shell
CC         = g++ -std=c++11 -O2 -Wall -fPIC -Xlinker -zmuldefs -Wno-unused-local-typedefs -pthread -Wno-deprecated-declarations -m64
SHELL      = /bin/bash
RM         = rm -f

ifndef ROOTSYS
	ROOTINCLUDE = -I$(shell root-config --incdir)
	ROOTLIBS    = $(shell root-config --libs)
	ROOTCLING   = rootcling
else
	ROOTINCLUDE = -I$(shell $(ROOTSYS)/bin/root-config --incdir)
	ROOTLIBS    = $(shell $(ROOTSYS)/bin/root-config --libs)
	ROOTCLING   = $(ROOTSYS)/bin/rootcling
endif
EXTRA_ROOTLIBS=-lRooFit -lRooFitCore

# Extensions
SRCEXT     = cpp
HDREXT     = h
OBJEXT     = o
LIBEXT     = so

# Directories
SRCDIR     = src
HDRDIR     = include
OBJDIR     = build
LIBDIR     = lib
# Get files and make list of objects and libraries
SRCS      := $(shell find $(SRCDIR) -name '*.$(SRCEXT)')
HDRS      := $(shell find $(HDRDIR) -name '*.$(HDREXT)')
DLHD      := $(patsubst $(SRCDIR)/%.$(SRCEXT), -p %.$(HDREXT), $(SRCS))
OBJS      := $(patsubst $(SRCDIR)/%.$(SRCEXT), $(OBJDIR)/%.$(OBJEXT), $(SRCS))

# Where the output is
OUTPUT     = $(LIBDIR)/libRooFitCustom.$(LIBEXT)

# Compiler flags
CXXFLAGS   = -I$(HDRDIR) $(ROOTINCLUDE) $(ROOTLIBS) $(EXTRA_ROOTLIBS)

all : $(OUTPUT)
# Build libraries
$(LIBDIR)/lib%.$(LIBEXT) : $(OBJS) dict/Dict.o | $(LIBDIR)
	$(CC) $(CXXFLAGS) -shared $^ -o $@
# Build objects
$(OBJDIR)/%.$(OBJEXT) : $(SRCDIR)/%.$(SRCEXT) | $(OBJDIR)
	$(CC) $(CXXFLAGS) -c $< -o $@
dict/Dict.cxx : | dict
	$(ROOTCLING) -f $@ -c -I$(HDRDIR) $(DLHD) LinkDef.h
dict/Dict.o : dict/Dict.cxx
	$(CC) $(CXXFLAGS) -c $< -o $@
# Make directories
$(LIBDIR) $(OBJDIR) dict:
	mkdir -p $@
# Remove all the output
clean :
	$(RM) $(OUTPUT) $(OBJS) dict/*

