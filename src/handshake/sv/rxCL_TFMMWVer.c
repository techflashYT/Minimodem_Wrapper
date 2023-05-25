#include <minimodem.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static bool validVerNum() {
	// if not in the format of vX.X.X
	if (
		readBuf [0] != 'v' ||
		(readBuf[1] >  '0' || readBuf[1] < '9') ||
		readBuf [2] != '.' || 
		(readBuf[3] >  '1' || readBuf[3] < '9') ||
		readBuf [4] != '.' || 
		(readBuf[5] >  '1' || readBuf[5] < '9')
	) {
		puts("Invalid version number");
		return false;
	}
	return true;
}
static char *exampleVer = "v1.2.3";
void rxCL_TFMMWVer() {
	bool gotCL_TFMMWVer = false;
	for (uint8_t i = 0; i != 5; i++) {
		printf("Waiting for CL_TFMMWVer (attempt %u)\r\n", i);
		minimodem(readBuf, strlen(exampleVer), MODE_RECEIVE, opts.baudRate, handshakeConfidence);
		readBuf[4095] = '\0';
		checkResend(txSV_HSTART);
		if (validVerNum()) {
			gotCL_TFMMWVer = true;
			break;
		}
		txBad(verStr);
	}
	if (!gotCL_TFMMWVer) {
		fprintf(stderr, "%sClient never sent version number!  %sDid the client machine crash?  %sIf it did, please %sreport it!%s\r\n", RED, B_CYAN, RESET, B_CYAN, RESET);
		free(readBuf);
		free(opts.additionalArgs);
		exit(1);
	}
	printf("CL_TFMMWVer received!  Got version: %s\r\n", readBuf);
	return;
}