# Much of this makefile is nicked from RapidFit and slimmed down considerably
# Compiler and shell
CC         = g++
SHELL      = /bin/bash
RM         = rm -f

# Include ROOT files as system headers as they're NOT standards complient and we do not want to waste time fixing them!
# ROOT has some broken backwards compatability for OSX so won't claim to be a set of system headers
ROOTCFLAGS = -L$(ROOTSYS)/lib $(shell $(ROOTSYS)/bin/root-config --cflags | awk -F "-I" '{print $$1" -isystem"$$2}' )
ROOTLIBS   = -L$(ROOTSYS)/lib $(shell $(ROOTSYS)/bin/root-config --libs)
EXTRA_ROOTLIBS=-lRooFit -lRooStats -lRooFitCore

# Extensions
SRCEXT     = cpp
HDREXT     = h
OBJEXT     = o
LIBEXT     = so

# Directories
PWD        = $(shell pwd)
SRCDIR     = $(PWD)/src
BINSRCDIR  = app
LIBSRCDIR  = lib
HDRDIR     = $(PWD)/include
OBJDIR     = $(PWD)/build
LIBDIR     = $(PWD)/lib
BINDIR     = $(PWD)/bin
# Get files and make list of objects and libraries
BINSRCS   := $(shell find $(SRCDIR)/$(BINSRCDIR) -name '*.$(SRCEXT)')
LIBSRCS   := $(shell find $(SRCDIR)/$(LIBSRCDIR) -name '*.$(SRCEXT)')
HDRS      := $(shell find $(HDRDIR) -name '*.$(HDREXT)')
LIBS      := $(patsubst $(SRCDIR)/$(LIBSRCDIR)/%.$(SRCEXT), $(LIBDIR)/lib%.$(LIBEXT), $(LIBSRCS))
BINS      := $(patsubst $(SRCDIR)/$(BINSRCDIR)/%.$(SRCEXT), $(BINDIR)/%, $(BINSRCS))

# Where the output is
OUTPUT     = $(OBJDIR)/*/*.$(OBJEXT) $(OBJDIR)/*.$(OBJEXT) $(LIBDIR)/*.$(LIBEXT) $(BINDIR)/*

# Compiler flags
CXXFLAGS   = -Wall -fPIC -I$(HDRDIR) $(ROOTCFLAGS) 
LIBFLAGS   = -L$(LIBDIR) $(ROOTLIBS) -lCloneInfo -lCloneTagger -lprogbar -lplotmaker -lGetTree -lboost_program_options $(EXTRA_ROOTLIBS) 

all : $(BINS)
libs : $(LIBS)
# Build binaries
$(BINDIR)/% : $(OBJDIR)/$(BINSRCDIR)/%.$(OBJEXT) $(LIBS)
	$(CC) $(LIBFLAGS) $^ $(LIBS) -o $@
# Build libraries
$(LIBDIR)/lib%.$(LIBEXT) : $(OBJDIR)/$(LIBSRCDIR)/%.$(OBJEXT)
	$(CC) -shared $< -o $@
# Build objects
$(OBJDIR)/%.$(OBJEXT) : $(SRCDIR)/%.$(SRCEXT)
	$(CC) $(CXXFLAGS) -c $< -o $@
# Remove all the output
clean :
	$(RM) $(OUTPUT)
