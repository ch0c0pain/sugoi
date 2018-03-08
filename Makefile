NAME := $(shell pwd | rev | cut -d/ -f 1 | rev)
BUILD := build
PREFIX :=
DEPS :=
CFLAGS := -O2 -Wall -std=c89 $(shell pkg-config --cflags $(DEPS)) -c
LDFLAGS := $(shell pkg-config --libs $(DEPS))

SOURCES := $(wildcard src/*.c)
OBJECTS := $(addprefix $(BUILD)/,$(notdir $(SOURCES:.c=.o)))

.PHONY: all

all: $(BUILD) $(BUILD)/$(NAME)

$(BUILD):
	mkdir -p $@

$(BUILD)/$(NAME): $(OBJECTS)
	gcc -o $@ $(OBJECTS) $(LDFLAGS)

$(BUILD)/%.o: src/%.c
	gcc -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -rf $(BUILD)
