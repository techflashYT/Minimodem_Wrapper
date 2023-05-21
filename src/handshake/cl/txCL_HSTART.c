#include <stdio.h>
#include <string.h>
#include <minimodem.h>
void txCL_HSTART() {
	printf("Sending CL_HSTART...\r\n");
	minimodem(handshakeStr, strlen(handshakeStr), MODE_TRANSMIT, handshakeBaudRate, 0);
	printf("Sent CL_HSTART!\r\n");
	return;
}