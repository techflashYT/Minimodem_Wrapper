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
void handshakeSv() {
	printf("Running as %sserver%s.\r\n", B_CYAN, RESET);
	// wait for a CL_HSTART
	bool gotCL_HSTART = false;
	for (uint8_t i = 0; i != 5; i++) {
		printf("Waiting for CL_HSTART (attempt %u)\r\n", i);
		minimodem(readBuf, 256, MODE_RECEIVE, handshakeBaudRate, handshakeConfidence);
		if (strncmp(readBuf, handshakeStr, strlen(handshakeStr)) == 0) {
			gotCL_HSTART = true;
			break;
		}
		
		printf("Huh?  We got \"\"\"%s\"\"\" when we expected \"\"\"%s\"\"\".  Retrying...\r\n", readBuf, handshakeStr);
		minimodem(resendFileName, 256, MODE_TRANSMIT, handshakeBaudRate, handshakeConfidence);
	}
	printf("CL_HSTART received!\r\n");
	// SV_HSTART
	for (uint8_t i = 0; i != 5; i++) {
		printf("Sending SV_HSTART (attempt %u)\r\n", i);
		printf("Sending SV_HSTART...\r\n");
		minimodem(handshakeFileName, 54, MODE_TRANSMIT, handshakeBaudRate, 0);
		printf("Sent SV_HSTART!\r\n");
		minimodem(readBuf, 256, MODE_RECEIVE, handshakeBaudRate, handshakeConfidence);
		if (strncmp(readBuf, handshakeStr, strlen(handshakeStr)) == 0) {
			gotCL_HSTART = true;
			break;
		}
		if (strstr(readBuf, "RESEND") != 0) {// contains RESEND
			// resend
			printf("Server asked for resend, sending handshake again...\r\n");
			minimodem(handshakeFileName, 54, MODE_TRANSMIT, handshakeBaudRate, 0);
			printf("Sent\r\n");
			continue;
		}
		printf("Huh?  We got \"\"\"%s\"\"\" when we expected \"\"\"%s\"\"\".  Retrying...\r\n", readBuf, handshakeStr);
		minimodem(resendFileName, 256, MODE_TRANSMIT, handshakeBaudRate, handshakeConfidence);
	}
	if (!gotCL_HSTART) {
		fprintf(stderr, "%sClient never initiated handshake!  %sAre you running this program on both machines?  %sIf you are, maybe check if your setup works properly %s(see wiki)%s.%s\r\n", RED, B_CYAN, RESET RED, RESET, RED, RESET);
		free(readBuf);
		abort();
	}
}