#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <minimodem.h>
void txSV_HSTART() {
	puts("Sending SV_HSTART...");
	minimodem(handshakeStr, strlen(handshakeStr), MODE_TRANSMIT, opts.baudRate, 0);
	return;
}