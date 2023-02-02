#include <stdio.h>
#include <minimodem.h>
void txCL_HSTART() {
	printf("Sending CL_HSTART...\r\n");
	minimodem(handshakeFileName, 54, MODE_TRANSMIT, handshakeBaudRate, 0);
	printf("Sent CL_HSTART!\r\n");
	return;
}