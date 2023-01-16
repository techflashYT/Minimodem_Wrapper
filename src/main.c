#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <minimodem.h>
char   *handshakeStr      = "Techflash Software Minimodem Wrapper\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"; // banking on at least *1* of these zeroes has to go through, even if it's noisy, because otherwise, we're screwed, the listen program will just run off into junk
char   *resendStr         = "RESEND_RESEND_RESEND_RESEND_RESEND_RESEND_RESEND_RESEND_RESEND_RESEND_RESEND_RESEND_RESEND_RESEND"; // we check for the word "RESEND", so it's very likely that at least one of these will go through correctly, otherwise, we're screwed
uint8_t*handshakeFileName;
uint8_t*resendFileName;
int main(int argc, char* argv[]) {
	printf("%sTechflash%s %sMiniModem Data Transfer Wrapper %sv%u.%u.%u\x1b[0m\r\n", B_YEL, RESET, B_CYAN, RESET GREEN, VER_MA, VER_MI, VER_PA);

	options_t opts = figureOutArgs(argc, argv);
	printf("%sOptions Parsed%s\r\n", RESET GREEN, RESET);

	handshakeFileName = malloc(34);
	resendFileName    = malloc(34);
	strcpy(handshakeFileName, "/tmp/TechflashMinimodemTmp_XXXXXX");
	strcpy(resendFileName, "/tmp/TechflashMinimodemTmp_XXXXXX");
	FILE   *handshakeFile     = fdopen(mkstemp(handshakeFileName), "w+");
	FILE   *resendFile        = fdopen(mkstemp(resendFileName), "w+");
	fwrite(handshakeStr, 54, 1, handshakeFile);
	fwrite(resendStr, strlen(resendStr), 1, resendFile);
	fclose(handshakeFile);
	fclose(resendFile);

	// CL_HSTART
	printf("Sending CL_HSTART...\r\n");
	minimodem(handshakeFileName, 54, MODE_TRANSMIT, 120, 0);
	printf("Sent CL_HSTART!\r\n");

	// SV_HSTART response?
	uint8_t *readBuf = malloc(256);
	if (readBuf == NULL) {
		fprintf(stderr, "%smalloc() call failed!%s.  Are you out of memory?\r\n", RED, RESET);
		abort();
	}
	bool SV_HSTART_worked = false;
	for (uint8_t i = 0; i != 5 && (!SV_HSTART_worked); i++) {
		printf("Waiting for SV_HSTART (attempt %u)\r\n", i);
		minimodem(readBuf, 256, MODE_RECEIVE, 120, 2.25);
		if (strncmp(readBuf, resendStr, strlen(resendStr)) == 0) {
			// resend
			printf("Server asked for resend, sending handshake again...\r\n");
			minimodem(handshakeFileName, 54, MODE_TRANSMIT, 120, 0);
			printf("Sent\r\n");
			break;
		}
		if (strncmp(readBuf, handshakeStr, strlen(handshakeStr)) == 0) {
			SV_HSTART_worked = true;
			break;
		}
		
		printf("Huh?  We got \"\"\"%s\"\"\" when we expected \"\"\"%s\"\"\".  Retrying...\r\n", readBuf, handshakeStr);
		minimodem(resendFileName, 256, MODE_TRANSMIT, 120, 2.25);
	}
	if (!SV_HSTART_worked) {
		fprintf(stderr, "%sOther machine never initiated handshake!  %sAre you running this program on both machines?  %sIf you are, maybe check if your setup works properly %s(see wiki)%s.%s\r\n", RED, B_CYAN, RESET RED, RESET, RED, RESET);
		free(readBuf);
		abort();
	}

	// minimodem(addr, size, opts.mode, opts.baudRate, opts.confidence);
	// munmap(addr, size);
	remove(handshakeFileName);
	free(readBuf);
	free(handshakeFileName);
	free(resendFileName);
	return 0;
}