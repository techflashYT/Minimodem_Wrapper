#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <minimodem.h>
int main(int argc, char* argv[]) {
	printf("\x1b[1;33mTechflash\x1b[0m \x1b[1;36mMiniModem Data Transfer Wrapper \x1b[0m\x1b[32mv%u.%u.%u\x1b[0m\r\n", VER_MA, VER_MI, VER_PA);
	options_t opts = figureOutArgs(argc, argv);
	printf("\x1b[0m\x1b[32mOptions Parsed\x1b[0m\r\n");
	// TODO: Check if any of the options aren't set.
	char fMode[3] = {0, 0, 0};
	if (opts.mode == MODE_RECEIVE) {
		fMode[0] = 'w';
		fMode[1] = '+';
	}
	if (opts.mode == MODE_TRANSMIT) {
		fMode[0] = 'r';
	}
	FILE *fd = fopen(opts.fileName, fMode);
	if (fd == NULL) {
		fputs("\x1b[31mERROR\x1b[0m: File does not exist!\r\n", stderr);
		abort();
	}
	size_t size = 65535;
	if (opts.mode == MODE_TRANSMIT) {
		fseek(fd, 0, SEEK_END);
		size = ftell(fd);
		fseek(fd, 0, SEEK_SET);
	}
	uint8_t *addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fileno(fd), 0);
	// handshake
	char *handshakeStr = "Techflash Software Minimodem Wrapper";
	minimodem(handshakeStr, strlen(handshakeStr), MODE_TRANSMIT, 10, 0); // confidence doesn't matter for sending, but the receiving end needs to wait for the handshake start
	minimodem(addr, size, opts.mode, opts.baudRate, opts.confidence);
	munmap(addr, size);
	fclose(fd);
	return 0;
}