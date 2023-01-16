#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <minimodem.h>
char   *handshakeStr      = "Techflash Software Minimodem Wrapper\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"; // banking on at least *1* of these zeroes has to go through, even if it's noisy, because otherwise, we're screwed, the listen program will just run off into junk
char   *resendStr         = "RESEND_RESEND_RESEND_RESEND_RESEND_RESEND_RESEND_RESEND_RESEND_RESEND_RESEND_RESEND_RESEND_RESEND"; // we check for the word "RESEND", so it's very likely that at least one of these will go through correctly, otherwise, we're screwed
uint8_t*handshakeFileName;
uint8_t*resendFileName;
FILE   *handshakeFile;
FILE   *resendFile;
uint8_t*readBuf;
options_t opts;
int main(int argc, char* argv[]) {
	printf("%sTechflash%s %sMiniModem Data Transfer Wrapper %sv%u.%u.%u\x1b[0m\r\n", B_YEL, RESET, B_CYAN, RESET GREEN, VER_MA, VER_MI, VER_PA);

	opts = figureOutArgs(argc, argv);
	printf("%sOptions Parsed%s\r\n", RESET GREEN, RESET);

	handshakeFileName = malloc(34);
	resendFileName    = malloc(34);
	strcpy(handshakeFileName, "/tmp/TechflashMinimodemTmp_XXXXXX");
	strcpy(resendFileName, "/tmp/TechflashMinimodemTmp_XXXXXX");
	handshakeFile     = fdopen(mkstemp(handshakeFileName), "w+");
	resendFile        = fdopen(mkstemp(resendFileName), "w+");
	
	fwrite(handshakeStr, 54, 1, handshakeFile);
	fwrite(resendStr, strlen(resendStr), 1, resendFile);
	fclose(handshakeFile);
	fclose(resendFile);

	readBuf = malloc(256);
	if (readBuf == NULL) {
		fprintf(stderr, "%smalloc() call failed!%s.  Are you out of memory?\r\n", RED, RESET);
		abort();
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
	free(handshakeFileName);
	free(resendFileName);
	return 0;
}