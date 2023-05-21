#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <minimodem.h>
#include <fec.h>
int main(int argc, char* argv[]) {
	printf("%sTechflash%s %sMiniModem Data Transfer Wrapper %sv%u.%u.%u\x1b[0m\r\n", B_YEL, RESET, B_CYAN, RESET GREEN, VER_MA, VER_MI, VER_PA);

	opts = figureOutArgs(argc, argv);
	readBuf = malloc(4096);
	printf("%sOptions Parsed%s\r\n", RESET GREEN, RESET);

	if (opts.mode == MODE_TRANSMIT) {
		handshakeCl();
	}
	if (opts.mode == MODE_RECEIVE) {
		handshakeSv();
	}
	
	// minimodem(addr, size, opts.mode, opts.baudRate, opts.confidence);
	// munmap(addr, size);
	free(readBuf);
	free(opts.additionalArgs);
	return 0;
}
