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
	txCL_HSTART();
	// SV_HSTART response?
	rxSV_HSTART();
}