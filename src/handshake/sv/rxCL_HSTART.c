#include <minimodem.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void rxCL_HSTART() {
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
	if (!gotCL_HSTART) {
		fprintf(stderr, "%sClient never initiated handshake!  %sAre you running this program on both machines?  %sIf you are, maybe check if your setup works properly %s(see wiki)%s.%s\r\n", RED, B_CYAN, RESET RED, RESET, RED, RESET);
		free(readBuf);
		free(opts.additionalArgs);
		exit(1);
	}
	puts("CL_HSTART received!\r\n");
	return;
}