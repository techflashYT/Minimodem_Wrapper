#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <minimodem.h>
#include <stdint.h>
extern char   *handshakeStr;
extern char   *resendStr;
extern uint8_t*handshakeFileName;
extern uint8_t*resendFileName;
extern uint8_t*readBuf;
void handshakeCl() {
	printf("Running as %sclient%s.\r\n", B_CYAN, RESET);
	// CL_HSTART
	printf("Sending CL_HSTART...\r\n");
	minimodem(handshakeFileName, 54, MODE_TRANSMIT, handshakeBaudRate, 0);
	printf("Sent CL_HSTART!\r\n");
	// SV_HSTART response?
	bool SV_HSTART_worked = false;
	for (uint8_t i = 0; i != 5; i++) {
		printf("Waiting for SV_HSTART (attempt %u)\r\n", i);
		minimodem(readBuf, 256, MODE_RECEIVE, handshakeBaudRate, handshakeConfidence);
		if (strstr(readBuf, "RESEND") != 0) {// contains RESEND
			// resend
			printf("Server asked for resend, sending handshake again...\r\n");
			minimodem(handshakeFileName, 54, MODE_TRANSMIT, handshakeBaudRate, 0);
			printf("Sent\r\n");
			continue;
		}
		if (strncmp(readBuf, handshakeStr, strlen(handshakeStr)) == 0) {
			SV_HSTART_worked = true;
			break;
		}
		
		printf("Huh?  We got \"\"\"%s\"\"\" when we expected \"\"\"%s\"\"\".  Retrying...\r\n", readBuf, handshakeStr);
		minimodem(resendFileName, 256, MODE_TRANSMIT, handshakeBaudRate, handshakeConfidence);
	}
	if (!SV_HSTART_worked) {
		fprintf(stderr, "%sServer never initiated handshake!  %sAre you running this program on both machines?  %sIf you are, maybe check if your setup works properly %s(see wiki)%s.%s\r\n", RED, B_CYAN, RESET RED, RESET, RED, RESET);
		free(readBuf);
		abort();
	}
	printf("SV_HSTART received!\r\n");
}