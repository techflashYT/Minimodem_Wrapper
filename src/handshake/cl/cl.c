#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <minimodem.h>
#include <stdint.h>
extern char   *handshakeStr;
extern char   *resendStr;
extern uint8_t*readBuf;
void handshakeCl() {
	printf("Running as %sclient%s.\r\n", B_CYAN, RESET);
	// send a CL_HSTART to let the server know "hey, i'm here!"
	txCL_HSTART();
	// SV_HSTART response?
	rxSV_HSTART();
	// yes, there's a server on the other side, but is the the right version?  Tell it ours and see what it thinks
	txCL_TFMMWVer();

}