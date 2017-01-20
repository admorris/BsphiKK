
# Compiler and shell
CC         = g++
SHELL      = /bin/bash
RM         = rm -f

ROOTCFLAGS = $(shell root-config --cflags)
ROOTLIBS   = $(shell root-config --libs)
ROOTLIBDIR = $(shell root-config --libdir)
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
# Location of compiled "common" libraries from ssh://git@gitlab.cern.ch:7999/admorris/common.git
COMMONDIR  = $(PWD)/common
COMHDRDIR  = $(COMMONDIR)/include
COMLIBDIR  = $(COMMONDIR)/lib

# Get files and make list of objects and libraries
BINSRCS   := $(shell find $(SRCDIR)/$(BINSRCDIR) -name '*.$(SRCEXT)')
LIBSRCS   := $(shell find $(SRCDIR)/$(LIBSRCDIR) -name '*.$(SRCEXT)')
HDRS      := $(shell find $(HDRDIR) -name '*.$(HDREXT)')
LIBS      := $(patsubst $(SRCDIR)/$(LIBSRCDIR)/%.$(SRCEXT), $(LIBDIR)/lib%.$(LIBEXT), $(LIBSRCS))
BINS      := $(patsubst $(SRCDIR)/$(BINSRCDIR)/%.$(SRCEXT), $(BINDIR)/%, $(BINSRCS))

# Where the output is
OUTPUT     = $(OBJDIR)/*/*.$(OBJEXT) $(OBJDIR)/*.$(OBJEXT) $(LIBDIR)/*.$(LIBEXT) $(BINDIR)/*
LOGDIRS    = latex/figs latex/results ntuples scripts/log scripts/tables

# Compiler flags
CXXFLAGS   = -Wall -fPIC -I$(HDRDIR) -I$(COMHDRDIR) $(ROOTCFLAGS)
COMLIBFLAGS = -L$(COMLIBDIR) $(shell make -sC $(COMMONDIR) libflags)
LIBFLAGS   = -Wl,--no-undefined -Wl,--no-allow-shlib-undefined -L$(LIBDIR) $(patsubst $(LIBDIR)/lib%.$(LIBEXT), -l%, $(LIBS)) $(COMLIBFLAGS) $(ROOTLIBS) $(EXTRA_ROOTLIBS) -lgsl -lgslcblas -lboost_program_options -Wl,-rpath,$(LIBDIR):$(COMLIBDIR):$(ROOTLIBDIR)

.PHONY: all $(COMMONDIR) clean

all : $(LIBS) $(BINS) | $(LOGDIRS)
$(COMMONDIR) :
	make -C $@
# Build binaries
$(BINDIR)/% : $(OBJDIR)/$(BINSRCDIR)/%.$(OBJEXT) $(LIBS) $(COMLIBS) | $(BINDIR)
	$(CC) $< -o $@ $(LIBFLAGS)
# Build libraries
$(LIBDIR)/lib%.$(LIBEXT) : $(OBJDIR)/$(LIBSRCDIR)/%.$(OBJEXT) $(HDRS) | $(LIBDIR)
	$(CC) -shared $< -o $@ -Wl,--as-needed $(COMLIBFLAGS) $(ROOTLIBS) $(EXTRA_ROOTLIBS)
# Build objects
$(OBJDIR)/%.$(OBJEXT) : $(SRCDIR)/%.$(SRCEXT) $(HDRS) $(COMMONDIR) | $(OBJDIR) $(OBJDIR)/$(LIBSRCDIR) $(OBJDIR)/$(BINSRCDIR)
	$(CC) $(CXXFLAGS) -c $< -o $@
# Make directories
$(LOGDIRS) $(BINDIR) $(LIBDIR) $(OBJDIR) $(OBJDIR)/$(LIBSRCDIR) $(OBJDIR)/$(BINSRCDIR) :
	mkdir -p $@
# Remove all the output
clean :
	$(RM) $(OUTPUT)
	make -C $(COMMONDIR) clean

