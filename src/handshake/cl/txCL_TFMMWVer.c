#include <stdio.h>
#include <string.h>
#include <minimodem.h>
void txCL_TFMMWVer() {
	printf("Sending CL_TFMMWVer...\r\n");
	minimodem(verStr, 7, MODE_TRANSMIT, opts.baudRate, 0);
	printf("Sent CL_TFMMWVer!\r\n");
	return;
}