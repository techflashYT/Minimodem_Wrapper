#include <stdio.h>
#include <minimodem.h>
extern char    *handshakeStr;
extern char    *resendStr;
extern uint8_t *readBuf;
void handshakeSv() {
	printf("Running as %sserver%s.\r\n", B_CYAN, RESET);
	// wait for a CL_HSTART
	rxCL_HSTART();
	// send a SV_HSTART
	txSV_HSTART();
	// wait for a CL_TFMMWVer
	rxCL_TFMMWVer();
}