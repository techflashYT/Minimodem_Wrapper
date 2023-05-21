CC       = gcc
WARN     = -Wall -Wextra -Wstack-protector -Wformat=2 -Wformat-security -Werror -Wno-error=unused-variable -Wno-pointer-sign
FEATURE  = -fdiagnostics-color=always -fstack-protector-all -fsanitize=address,undefined -march=core2
INCLUDES = -Isrc/include -Isrc/libs/libfec
LIBS     = -L src/libs/libfec/ -lm -l:libfec.a
# GLIBCDIR = /opt/TechflashSoftware/crossCompiler/x86_64-linux/lib
CFLAGS   = $(WARN) $(FEATURE) $(INCLUDES) -O0 -g -std=gnu2x
# CRTFILES = /opt/TechflashSoftware/crossCompiler/x86_64-linux/lib/crt1.o /opt/TechflashSoftware/crossCompiler/x86_64-linux/lib/crti.o /opt/TechflashSoftware/crossCompiler/x86_64-linux/lib/crtn.o -l:libc.so -lgcc -lasan -lm
vpath %.c src
vpath %.h src/include
SHELL=/bin/bash
compile  := $(patsubst src/%.c,build/%.o,$(shell find src -name '*.c' | grep -v "src/libs/"))
# compile  += "build/libs/reed-solomon-ecc/rs.c"
includes := $(shell find src/include -type f)
outFileName=tfModemTransfer

.SUFFIXES: .c .o
all: src/libs/libfec/libfec.so bin/$(outFileName)
	@echo "Built!"
src/libs/libfec/libfec.so:
	@cd src/libs/libfec; ./configure; make $(MAKEOPTS)
bin/$(outFileName): $(compile)
	@mkdir -p $(@D)
	@echo "CC    $^ => $@"
	@$(CC) $(CFLAGS) $^ $(LIBS) -o $@



build/%.o: %.c $(includes)
	@mkdir -p $(@D)
	@echo "CC    $< => $@"
	@$(CC) $(CFLAGS) -c $< -o $@
clean:
	@rm -rf build bin
