CC = gcc
LD = gcc

CFILES = $(shell find src/ -name '*.c')
CFLAGS = -O2 -g -Isrc/include -Wall -Wextra -fsanitize=address,undefined -include src/include/config.h
LDFLAGS= -lasan -lubsan

OBJ = $(patsubst src/%.c,build/%.o,$(CFILES))

all: bin/tfmmw

bin/tfmmw: $(OBJ)
	$(info $s  LD    $(subst build/,,$^) ==> $@)
	@mkdir -p $(@D)
	@$(LD) $(LDFLAGS) $^ -o $@

build/%.o: src/%.c
	$(info $s  CC    $(subst src/,,$^) $(subst build/,,$@))
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -rf bin build