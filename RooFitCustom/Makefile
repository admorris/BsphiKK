# Compiler and shell
CC         = g++
SHELL      = /bin/bash
RM         = rm -f

ifndef ROOTSYS
	ROOTCFLAGS = $(shell root-config --cflags)
	ROOTLIBS   = $(shell root-config --libs)
	ROOTCLING  = rootcling
else
	ROOTCFLAGS = $(shell $(ROOTSYS)/bin/root-config --cflags | awk -F "-I" '{print $$1" -isystem"$$2}' )
	ROOTLIBS   = $(shell $(ROOTSYS)/bin/root-config --libs)
	ROOTCLING  = $(ROOTSYS)/bin/rootcling
endif
EXTRA_ROOTLIBS=-lRooFit -lRooStats -lRooFitCore

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
DLHD      := $(patsubst $(SRCDIR)/%.$(SRCEXT), %.$(HDREXT), $(SRCS))
OBJS      := $(patsubst $(SRCDIR)/%.$(SRCEXT), $(OBJDIR)/%.$(OBJEXT), $(SRCS))

# Where the output is
OUTPUT     = $(LIBDIR)/libRooFitCustom.$(LIBEXT)

# Compiler flags
CXXFLAGS   = -Wall -fPIC -I$(HDRDIR) $(ROOTCFLAGS)
LIBFLAGS   = -Wl,--as-needed $(ROOTLIBS) $(EXTRA_ROOTLIBS)

all : $(OUTPUT)
# Build libraries
$(LIBDIR)/lib%.$(LIBEXT) : $(OBJS) dict/Dict.o | $(LIBDIR)
	$(CC) -shared $^ -o $@ $(LIBFLAGS)
# Build objects
$(OBJDIR)/%.$(OBJEXT) : $(SRCDIR)/%.$(SRCEXT) | $(OBJDIR)
	$(CC) $(CXXFLAGS) -c $< -o $@
dict/Dict.cxx : | dict
	$(ROOTCLING) -f $@ -c -I$(HDRDIR) -p $(DLHD) LinkDef.h
dict/Dict.o : dict/Dict.cxx
	$(CC) $(CXXFLAGS) -c $< -o $@
# Make directories
$(LIBDIR) $(OBJDIR) dict:
	mkdir -p $@
# Remove all the output
clean :
	$(RM) $(OUTPUT) $(OBJS) dict/*

