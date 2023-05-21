#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <minimodem.h>
#include <fec.h>
int main(int argc, char* argv[]) {
	printf("%sTechflash%s %sMiniModem Data Transfer Wrapper %sv%u.%u.%u\x1b[0m\r\n", B_YEL, RESET, B_CYAN, RESET GREEN, VER_MA, VER_MI, VER_PA);

	opts = figureOutArgs(argc, argv);
	printf("%sOptions Parsed%s\r\n", RESET GREEN, RESET);
	// test it
	uint8_t *data = malloc(64);
	strcpy(data, "hello world this is a very long string");

	size_t size = strlen(data) + 1;

	ecc_t eccEnc = eccCreate(25, size);
	uint8_t *encoded = 0;
	eccEncode(eccEnc, data, size, &encoded);

	ecc_t eccDec = eccCreate(25, eccEnc->parityBytes + size);
	uint8_t *decoded = 0;
	eccDecode(eccDec, encoded, eccEnc->parityBytes + size, &decoded);


	if (strcmp(data, decoded) != 0) {
		fprintf(stderr, "ecc test failed\r\n");
		exit(1);
	}


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
