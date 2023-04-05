CC := ../../script/plugcc
LD := ../../script/plugld
BUILDIR := build

PROJECT_NAME := $(notdir $(shell pwd))

SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:src/%.c=$(BUILDIR)/%.o)

$(PROJECT_NAME).ndl: $(BUILDIR)/$(PROJECT_NAME).elf
	elf2ndl $< $@

$(BUILDIR)/$(PROJECT_NAME).elf: $(OBJS)
	$(LD) $(OBJS) -o $@

$(BUILDIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(PROJECT_NAME).ndl
	rm $(BUILDIR)/*
