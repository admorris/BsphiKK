# Location of "common" libraries from ssh://git@gitlab.cern.ch:7999/admorris/common.git
COMMONDIR  = $(PWD)/../common
# Compiler and shell
CC         = g++
SHELL      = /bin/bash
RM         = rm -f

ifndef ROOTSYS
	ROOTCFLAGS = $(shell root-config --cflags)
	ROOTLIBS   = $(shell root-config --libs)
else
	ROOTCFLAGS = $(shell $(ROOTSYS)/bin/root-config --cflags | awk -F "-I" '{print $$1" -isystem"$$2}' )
	ROOTLIBS   = $(shell $(ROOTSYS)/bin/root-config --libs)
endif
EXTRA_ROOTLIBS = -lRooFit -lRooStats -lRooFitCore

# Extensions
SRCEXT     = cpp
HDREXT     = h
OBJEXT     = o
LIBEXT     = so

# Directories
PWD        = $(shell pwd)
SRCDIR     = src
BINSRCDIR  = app
LIBSRCDIR  = lib
HDRDIR     = include
OBJDIR     = build
LIBDIR     = $(PWD)/lib
BINDIR     = bin
COMHDRDIR  = $(COMMONDIR)/include
COMLIBDIR  = $(COMMONDIR)/lib
# Get files and make list of objects and libraries
BINSRCS   := $(shell find $(SRCDIR)/$(BINSRCDIR) -name '*.$(SRCEXT)')
LIBSRCS   := $(shell find $(SRCDIR)/$(LIBSRCDIR) -name '*.$(SRCEXT)')
HDRS      := $(shell find $(HDRDIR) -name '*.$(HDREXT)')
LIBS      := $(patsubst $(SRCDIR)/$(LIBSRCDIR)/%.$(SRCEXT), $(LIBDIR)/lib%.$(LIBEXT), $(LIBSRCS))
BINS      := $(patsubst $(SRCDIR)/$(BINSRCDIR)/%.$(SRCEXT), $(BINDIR)/%, $(BINSRCS))
COMLIBS   := $(shell find $(COMLIBDIR) -name '*.$(LIBEXT)')

# Where the output is
OUTPUT     = $(OBJDIR)/*/*.$(OBJEXT) $(OBJDIR)/*.$(OBJEXT) $(LIBDIR)/*.$(LIBEXT) $(BINDIR)/*

# Compiler flags
CXXFLAGS   = -Wall -fPIC -I$(HDRDIR) -I$(COMHDRDIR) $(ROOTCFLAGS) -std=c++11
COMLIBFLAGS = -L$(COMLIBDIR) $(patsubst $(COMLIBDIR)/lib%.$(LIBEXT), -l%, $(COMLIBS))
LIBFLAGS   = -Wl,--as-needed -L$(LIBDIR) $(patsubst $(LIBDIR)/lib%.$(LIBEXT), -l%, $(LIBS)) $(COMLIBFLAGS) $(ROOTLIBS) $(EXTRA_ROOTLIBS) -lboost_program_options -lgsl -lgslcblas -Wl,-rpath=$(LIBDIR)

all : $(LIBS) $(BINS)
# Build binaries
$(BINDIR)/% : $(OBJDIR)/$(BINSRCDIR)/%.$(OBJEXT) $(LIBS) $(COMLIBS)
	$(CC) $< -o $@ $(LIBFLAGS)
# Build libraries
$(LIBDIR)/lib%.$(LIBEXT) : $(OBJDIR)/$(LIBSRCDIR)/%.$(OBJEXT) $(HDRS)
	$(CC) -shared $< -o $@ -Wl,--as-needed $(COMLIBFLAGS) $(ROOTLIBS) $(EXTRA_ROOTLIBS)
# Build objects
$(OBJDIR)/%.$(OBJEXT) : $(SRCDIR)/%.$(SRCEXT) $(HDRS)
	$(CC) $(CXXFLAGS) -c $< -o $@
# Remove all the output
clean :
	$(RM) $(OUTPUT)
