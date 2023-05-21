#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <minimodem.h>
void txSV_HSTART() {
	bool sentSV_HSTART = false;
	for (uint8_t i = 0; i != 5; i++) {
		printf("Sending SV_HSTART (attempt %u)\r\n", i);
		puts("Sending SV_HSTART...");
		minimodem(handshakeStr, strlen(handshakeStr), MODE_TRANSMIT, handshakeBaudRate, 0);
		puts("Sent SV_HSTART!");
		minimodem(readBuf, strlen(allGoodStr), MODE_RECEIVE, handshakeBaudRate, handshakeConfidence);
		if (strncmp(readBuf, allGoodStr, strlen(allGoodStr)) == 0) {
			sentSV_HSTART = true;
			break;
		}
		readBuf[4095] = '\0';
		if (strstr(readBuf, "RESEND") != 0) {// contains RESEND
			globalParityOffset += 15;
			// resend
			puts("Client asked for resend, sending handshake again...");
			minimodem(handshakeStr, strlen(handshakeStr), MODE_TRANSMIT, handshakeBaudRate, 0);
			puts("Sent");
			continue;
		}
		printf("Huh?  We got nonsense when we expected \"\"\"%s\"\"\".  Retrying...\r\n", handshakeStr);
		minimodem(resendStr, 256, MODE_TRANSMIT, handshakeBaudRate, handshakeConfidence);
	}
	if (!sentSV_HSTART) {
		fprintf(stderr, "%sClient never confirmed SV_HSTART!  %sCheck your connection and try again.%s\r\n", RED, B_CYAN, RESET);
		free(readBuf);
		free(opts.additionalArgs);
		exit(1);
	}
	return;
}