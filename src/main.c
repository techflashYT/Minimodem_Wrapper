#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <minimodem.h>
#include "../libs/reed-solomon-ecc/rs.h"
int main(int argc, char* argv[]) {
	printf("%sTechflash%s %sMiniModem Data Transfer Wrapper %sv%u.%u.%u\x1b[0m\r\n", B_YEL, RESET, B_CYAN, RESET GREEN, VER_MA, VER_MI, VER_PA);

	opts = figureOutArgs(argc, argv);
	printf("%sOptions Parsed%s\r\n", RESET GREEN, RESET);
	// initialize the reed solomon lib I'm using
	fec_init();
	
	// test it
	uint8_t *data = malloc(64);
	strcpy(data, "hello world");
	eccEncode(data, strlen(data) + 1, 25);

	if (opts.mode == MODE_TRANSMIT) {
		handshakeCl();
	}
	if (opts.mode == MODE_RECEIVE) {
		handshakeSv();
	}
	
	// minimodem(addr, size, opts.mode, opts.baudRate, opts.confidence);
	// munmap(addr, size);
	remove(handshakeFileName);
	remove(resendFileName);
	free(readBuf);
	free(opts.additionalArgs);
	free(handshakeFileName);
	free(resendFileName);
	return 0;
}