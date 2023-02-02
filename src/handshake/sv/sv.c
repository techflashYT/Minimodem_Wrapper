#include <stdio.h>
#include <minimodem.h>
extern char    *handshakeStr;
extern char    *resendStr;
extern uint8_t *handshakeFileName;
extern uint8_t *resendFileName;
extern uint8_t *readBuf;
void handshakeSv() {
	printf("Running as %sserver%s.\r\n", B_CYAN, RESET);
	// wait for a CL_HSTART
	rxCL_HSTART();
	// SV_HSTART
	txSV_HSTART();
}