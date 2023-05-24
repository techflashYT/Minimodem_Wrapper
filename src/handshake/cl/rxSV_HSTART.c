#include <minimodem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void rxSV_HSTART() {
	bool SV_HSTART_worked = false;
	for (uint8_t i = 0; i != 5; i++) {
		printf("Waiting for SV_HSTART (attempt %u)\r\n", i);
		minimodem(readBuf, strlen(resendStr), MODE_RECEIVE, handshakeBaudRate, handshakeConfidence);
		readBuf[4095] = '\0'; // null terminate just in case
		if (strstr(readBuf, "RESEND") != 0) {// contains RESEND
			// resend
			printf("Server asked for resend, sending handshake again...\r\n");
			txCL_HSTART();
			continue;
		}
		if (strncmp(readBuf, handshakeStr, strlen(handshakeStr)) == 0) {
			SV_HSTART_worked = true;
			break;
		}
		
		printf("Huh?  We got nonsense when we expected \"\"\"%s\"\"\".  Retrying...\r\n", handshakeStr);
		minimodem(resendStr, strlen(resendStr), MODE_TRANSMIT, handshakeBaudRate, handshakeConfidence);
	}
	if (!SV_HSTART_worked) {
		fprintf(stderr, "%sServer never initiated handshake!  %sAre you running this program on both machines?  %sIf you are, maybe check if your setup works properly %s(see wiki)%s.%s\r\n", RED, B_CYAN, RESET RED, RESET, RED, RESET);
		free(readBuf);
		free(opts.additionalArgs);
		exit(1);
	}
	printf("SV_HSTART received!\r\n");
}