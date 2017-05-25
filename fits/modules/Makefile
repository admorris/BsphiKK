ASSEMBLE = ./assemble.sh

SRCDIR = ../src/
SRCEXT = list
XMLDIR = ../results/


SRCS := $(shell find $(SRCDIR) -type f -name '*.$(SRCEXT)')
XMLS := $(patsubst $(SRCDIR)%.$(SRCEXT), $(XMLDIR)%.xml, $(SRCS))
DIRS := $(patsubst $(SRCDIR)%, $(XMLDIR)%, $(shell find $(SRCDIR) -mindepth 1 -type d))

all : $(XMLS) Makefile

.SECONDEXPANSION:
$(XMLDIR)%.xml : $(SRCDIR)%.$(SRCEXT) $$(shell echo `< $$(SRCDIR)%.$$(SRCEXT)`) dirs
	@echo "Building $@"
	@$(ASSEMBLE) `< $<` > $@

dirs :
	@mkdir -p $(DIRS)

clean :
	@rm -rf $(XMLS)

