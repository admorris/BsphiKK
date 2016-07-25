# RapidFit directory
RAPIDDIR   = $(PWD)/../../RapidFit
# Location of "common" libraries from ssh://git@gitlab.cern.ch:7999/admorris/common.git
COMMONDIR  = $(PWD)/../../common

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
#EXTRA_ROOTLIBS=-lRooFit -lRooStats -lRooFitCore

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
LIBDIR     = lib
BINDIR     = bin

# RapidFit directories
RFINCDIR    = $(RAPIDDIR)/framework/include
RFINCPDFDIR = $(RAPIDDIR)/pdfs/include
RFINCUTILS  = $(RAPIDDIR)/utils/include
RFLIBDIR    = $(RAPIDDIR)/lib

# Common directories
COMHDRDIR  = $(COMMONDIR)/include
COMLIBDIR  = $(COMMONDIR)/lib

# Get files and make list of objects and libraries
BINSRCS   := $(shell find $(SRCDIR)/$(BINSRCDIR) -name '*.$(SRCEXT)')
LIBSRCS   := $(shell find $(SRCDIR)/$(LIBSRCDIR) -name '*.$(SRCEXT)')
HDRS      := $(shell find $(HDRDIR) -name '*.$(HDREXT)')
LIBS      := $(patsubst $(SRCDIR)/$(LIBSRCDIR)/%.$(SRCEXT), $(LIBDIR)/lib%.$(LIBEXT), $(LIBSRCS))
BINS      := $(patsubst $(SRCDIR)/$(BINSRCDIR)/%.$(SRCEXT), $(BINDIR)/%, $(BINSRCS))
RFLIBS    := $(shell find $(RFLIBDIR) -name '*.$(LIBEXT)')
COMLIBS   := $(shell find $(COMLIBDIR) -name '*.$(LIBEXT)')

# Where the output is
OUTPUT     = $(OBJDIR)/*/*.$(OBJEXT) $(OBJDIR)/*.$(OBJEXT) $(LIBDIR)/*.$(LIBEXT) $(BINDIR)/*

# Compiler flags
CXXFLAGS   = -Wall -fPIC -I$(HDRDIR) -I$(RFINCDIR) -I$(RFINCPDFDIR) -I$(RFINCUTILS) $(ROOTCFLAGS) 
LIBFLAGS   = -L$(LIBDIR) -L$(COMLIBDIR) -L$(RFLIBDIR) $(ROOTLIBS) -lboost_program_options $(EXTRA_ROOTLIBS) 

all : dirs $(LIBS) $(BINS)
dirs :
	mkdir -p bin build/app build/lib lib
# Build binaries
$(BINDIR)/% : $(OBJDIR)/$(BINSRCDIR)/%.$(OBJEXT) $(LIBS)
	$(CC) $(LIBFLAGS) $^ $(LIBS) $(COMLIBS) $(RFLIBS) -o $@
# Build libraries
$(LIBDIR)/lib%.$(LIBEXT) : $(OBJDIR)/$(LIBSRCDIR)/%.$(OBJEXT)
	$(CC) -shared $< -o $@
# Build objects
$(OBJDIR)/%.$(OBJEXT) : $(SRCDIR)/%.$(SRCEXT)
	$(CC) $(CXXFLAGS) -c $< -o $@
# Remove all the output
clean :
	$(RM) $(OUTPUT)
