ASSEMBLE = ./assemble.sh

SRCDIR = ../src/
SRCEXT = list
XMLDIR = ../results/


SRCS := $(shell find $(SRCDIR) -type f -name '*.$(SRCEXT)')
XMLS := $(patsubst $(SRCDIR)%.$(SRCEXT), $(XMLDIR)%.xml, $(SRCS))
DIRS := $(patsubst $(SRCDIR)%, $(XMLDIR)%, $(shell find $(SRCDIR) -mindepth 1 -type d))

all : $(XMLS) Makefile

.SECONDEXPANSION:
$(XMLDIR)%.xml : $(SRCDIR)%.$(SRCEXT) $$(shell echo `< $$(SRCDIR)%.$$(SRCEXT)`) | $(DIRS)
	@echo "Building $@"
	@$(ASSEMBLE) `< $<` > $@

%.xml : $(patsubst $(XMLDIR)%.xml, $(SRCDIR)%.$(SRCEXT), $@)
	@for file in `< $<`; do ls $${file} 1>/dev/null; done

$(DIRS) :
	mkdir -p $@

clean :
	@for dir in $(DIRS); do rm -f $${dir}/*.xml; done


