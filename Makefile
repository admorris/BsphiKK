# Much of this makefile is nicked from RapidFit and slimmed down considerably
# Compiler and shell
CC         = g++
SHELL      = /bin/bash
RM         = rm -f

# Include ROOT files as system headers as they're NOT standards complient and we do not want to waste time fixing them!
# ROOT has some broken backwards compatability for OSX so won't claim to be a set of system headers
ROOTCFLAGS = -L$(ROOTSYS)/lib $(shell $(ROOTSYS)/bin/root-config --cflags | awk -F "-I" '{print $$1" -isystem"$$2}' )
ROOTLIBS   = -L$(ROOTSYS)/lib $(shell $(ROOTSYS)/bin/root-config --libs)
EXTRALIBS  = -lTreePlayer -lHtml -lThread -lMinuit -lMinuit2 -lRooFit -lRooStats -lRooFitCore -lFoam $(shell if [ "$(shell root-config --features | grep mathmore)" == "" ]; then echo "" ; else echo "-lMathMore" ; fi)

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
BINDIR     = bin

# Get files and make list of objects and libraries
SRCS      := $(shell find $(SRCDIR) -name '*.$(SRCEXT)')
HDRS      := $(shell find $(HDRDIR) -name '*.$(HDREXT)')
OBJS      := $(patsubst $(SRCDIR)/%.$(SRCEXT),$(OBJDIR)/%.$(OBJEXT),$(SRCS))
LIBS      := $(patsubst $(SRCDIR)/%.$(SRCEXT),$(LIBDIR)/lib%.$(LIBEXT),$(SRCS))

# Where the output is
OUTPUT     = $(OBJDIR)/*.$(OBJEXT) $(LIBDIR)/*.$(LIBEXT)

# Compiler flags
CXXFLAGS   = -Wall -fPIC -I$(HDRDIR) $(ROOTCFLAGS)
LIBFLAGS   = -L$(LIBDIR) $(ROOTLIBS) $(EXTRALIBS) -lprogbar -lCloneTagger -lCloneInfo -lplotmaker

# Make the libraries
all : $(LIBS) $(BINDIR)/AddBranches $(BINDIR)/FlagClones $(BINDIR)/CutEff $(BINDIR)/PlotBsMasses
# Make object files
objects : $(OBJS)
# Build binaries
$(BINDIR)/% : $(OBJDIR)/%.$(OBJEXT) 
	$(CC) $(LIBFLAGS) $^ $(LIBS) -o $@
#	$(CC) $(LIBFLAGS) $(OBJDIR)/$(patsubst $(BINDIR)/%,%,$@).$(OBJEXT) -o $@
# Build libraries
$(LIBDIR)/lib%.$(LIBEXT) : $(OBJDIR)/%.$(OBJEXT)
	$(CC) -shared $< -o $@
# Build objects
$(OBJDIR)/%.$(OBJEXT) : $(SRCDIR)/%.$(SRCEXT)
	$(CC) $(CXXFLAGS) -c $< -o $@
# Remove all the output
clean :
	$(RM) $(OUTPUT)
