# Compiler and shell
CC         = g++
SHELL      = /bin/bash
RM         = rm -f

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

# Where the output is
OUTPUT     = $(OBJDIR)/*/*.$(OBJEXT) $(OBJDIR)/*.$(OBJEXT) $(LIBDIR)/*.$(LIBEXT) $(BINDIR)/*
LOGDIRS    = latex/figs latex/results ntuples scripts/log scripts/tables

# ROOT
ROOTCFLAGS     = $(shell root-config --cflags)
ROOTLIBS       = $(shell root-config --libs)
ROOTLIBDIR     = $(shell root-config --libdir)
EXTRA_ROOTLIBS = -lRooFit -lRooStats -lRooFitCore
ROOTLIBFLAGS   = -L$(ROOTLIBDIR) -Wl,--as-needed $(ROOTLIBS) $(EXTRA_ROOTLIBS) -Wl,-rpath,$(ROOTLIBDIR)

# Submodules
COMMONDIR   = common
COMCXXFLAGS = $(shell make -sC $(COMMONDIR) cflags)
COMLIBS     = $(shell make -sC $(COMMONDIR) libs)
COMLIBDIR   = $(shell make -sC $(COMMONDIR) libdir)
COMLIBFLAGS = -L$(COMLIBDIR) -Wl,--as-needed $(COMLIBS) -Wl,-rpath,$(COMLIBDIR)

CLONEDIR      = CloneTagger
CLONECXXFLAGS = $(shell make -sC $(CLONEDIR) cflags)
CLONELIBS     = $(shell make -sC $(CLONEDIR) libs)
CLONELIBDIR   = $(shell make -sC $(CLONEDIR) libdir)
CLONELIBFLAGS = -L$(CLONELIBDIR) -Wl,--as-needed $(CLONELIBS) -Wl,-rpath,$(CLONELIBDIR)

ERFDIR = ExtraRooFit
ERFCXXFLAGS = $(shell make -sC $(ERFDIR) cflags)
ERFLIBS     = $(shell make -sC $(ERFDIR) libs)
ERFLIBDIR   = $(shell make -sC $(ERFDIR) libdir)
ERFLIBFLAGS = -L$(ERFLIBDIR) -Wl,--as-needed $(ERFLIBS) -Wl,-rpath,$(ERFLIBDIR)

SUBMODULES = $(COMMONDIR) $(CLONEDIR) $(ERFDIR)

# GSL and Boost
EXTRA_LIBFLAGS = -Wl,--as-needed -lgsl -lgslcblas -lboost_program_options

# Get files and make list of objects and libraries
BINSRCS   := $(shell find $(SRCDIR)/$(BINSRCDIR) -name '*.$(SRCEXT)')
LIBSRCS   := $(shell find $(SRCDIR)/$(LIBSRCDIR) -name '*.$(SRCEXT)')
HDRS      := $(shell find $(HDRDIR) -name '*.$(HDREXT)')
LIBS      := $(patsubst $(SRCDIR)/$(LIBSRCDIR)/%.$(SRCEXT), $(LIBDIR)/lib%.$(LIBEXT), $(LIBSRCS))
BINS      := $(patsubst $(SRCDIR)/$(BINSRCDIR)/%.$(SRCEXT), $(BINDIR)/%, $(BINSRCS))

# Compiler flags
CXXFLAGS          = -Wall -fPIC -I$(HDRDIR) $(COMCXXFLAGS) $(CLONECXXFLAGS) $(ERFCXXFLAGS) $(ROOTCFLAGS)
LOCAL_LIBFLAGS    = -L$(LIBDIR) $(patsubst $(LIBDIR)/lib%.$(LIBEXT), -l%, $(LIBS)) -Wl,-rpath,$(PWD)/$(LIBDIR)
EXTERNAL_LIBFLAGS = $(COMLIBFLAGS) $(CLONELIBFLAGS) $(ROOTLIBFLAGS) $(EXTRA_LIBFLAGS) $(ERFLIBFLAGS)
LIBFLAGS          = -Wl,--no-undefined -Wl,--no-allow-shlib-undefined $(LOCAL_LIBFLAGS) $(EXTERNAL_LIBFLAGS)

.PHONY: all $(SUBMODULES) clean

all : $(LIBS) $(BINS) | $(LOGDIRS)
$(SUBMODULES) :
	make -C $@
# Build binaries
$(BINDIR)/% : $(OBJDIR)/$(BINSRCDIR)/%.$(OBJEXT) $(LIBS) | $(BINDIR) $(SUBMODULES)
	@echo "Linking $@"
	@$(CC) $< -o $@ $(LIBPREFIX) $(LIBFLAGS)
# Build libraries
$(LIBDIR)/lib%.$(LIBEXT) : $(OBJDIR)/$(LIBSRCDIR)/%.$(OBJEXT) $(HDRS) | $(LIBDIR) $(SUBMODULES)
	@echo "Making shared object $@"
	@$(CC) -shared $< -o $@ $(EXTERNAL_LIBFLAGS)
# Build objects
$(OBJDIR)/%.$(OBJEXT) : $(SRCDIR)/%.$(SRCEXT) $(HDRS) | $(OBJDIR) $(OBJDIR)/$(LIBSRCDIR) $(OBJDIR)/$(BINSRCDIR)
	@echo "Compiling $@"
	@$(CC) -c $< -o $@ $(CXXFLAGS)
# Make directories
$(LOGDIRS) $(BINDIR) $(LIBDIR) $(OBJDIR) $(OBJDIR)/$(LIBSRCDIR) $(OBJDIR)/$(BINSRCDIR) :
	mkdir -p $@
# Remove all the output
clean :
	$(RM) $(OUTPUT)
	make -C $(COMMONDIR) clean
	make -C $(CLONEDIR) clean

