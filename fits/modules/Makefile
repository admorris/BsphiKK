ASSEMBLE = ./assemble.sh

SRCDIR = ../src/
SRCEXT = list
XMLDIR = ../

SRCS := $(shell find $(SRCDIR) -type f -name '*.$(SRCEXT)')
XMLS := $(patsubst $(SRCDIR)%.$(SRCEXT), $(XMLDIR)%.xml, $(SRCS))

all : $(XMLS) Makefile

$(XMLDIR)%.xml : $(SRCDIR)%.$(SRCEXT)
	@echo "Building $@"
	@$(ASSEMBLE) `< $<` > $@

clean :
	@rm -rf $(XMLS)
