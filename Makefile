CC = gcc
LD = gcc

CFILES = $(shell find src/ -name '*.c')

OBJ = $(patsubst src/%.c,build/%.o,$(CFILES))

bin/tfmmw: $(OBJ)
	$(info $s  LD    $(subst build/,,$^) ==> $@)
	@mkdir -p $(@D)
	@$(LD) $(LDFLAGS) $^ -o $@

build/%.o: src/%.c
	$(info $s  CC    $(subst src/,,$^) $(subst build/,,$@))
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $^ -o $@