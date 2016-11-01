ASSEMBLE = ./assemble.sh

SRCDIR = ../src/
SRCEXT = list
XMLDIR = ../


SRCS := $(shell find $(SRCDIR) -type f -name '*.$(SRCEXT)')
XMLS := $(patsubst $(SRCDIR)%.$(SRCEXT), $(XMLDIR)%.xml, $(SRCS))

all : $(XMLS) Makefile

.SECONDEXPANSION:
$(XMLDIR)%.xml : $(SRCDIR)%.$(SRCEXT) $$(shell echo `< $$(SRCDIR)%.$$(SRCEXT)`)
	@echo "Building $@"
	@$(ASSEMBLE) `< $<` > $@

clean :
	@rm -rf $(XMLS)
