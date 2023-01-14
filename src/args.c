#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <minimodem.h>
options_t retVal;
static void usage(const char *progName) {
	fprintf(stderr,
		"USAGE:\r\n"
		"	`%s\' **file name for sending from/receiving to**  **`t\' for transmit or `r\' for receive**  **baud rate** **confidence value**\r\n"
		"EXAMPLES:\r\n"
		"	To transmit `inputFile.bin\' at 300 baud with 2.75 confidence for a noisy cable: %s inputFile.bin t 300 2.75\r\n"
		"	To receive into `outputFile.bin\' at 1500 baud with 5 confidence for a very clean signal: %s outFile.bin r 1500 5\r\n",
		progName, progName, progName
	);
	exit(1);
}
options_t figureOutArgs(int argc, char *argv[]) {
	if (argc != 5) {
		usage(argv[0]);
	}
	memset(&retVal, '\0', sizeof(retVal));
	retVal.fileName = argv[1];
	if (argv[2][0] != 'r' && argv[2][0] != 't') {
		fputs("\x1b[31mERROR\x1b[0m: Transmit/Receive value must be exactly `t\' or `r\'.\r\n", stderr);
		abort();
	}
	if      (argv[2][0] == 'r') {retVal.mode = MODE_RECEIVE;}
	else if (argv[2][0] == 't') {retVal.mode = MODE_TRANSMIT;}
	for (uint16_t i = 0; i != strlen(argv[3]); i++) {
		if (!(isdigit(argv[3][i])) && argv[3][i] != '.') {
			fputs("\x1b[31mERROR\x1b[0m: third argument must be an integer or decimal number.", stderr);
			abort();
		}
	}
	retVal.baudRate   = atoi(argv[3]);
	for (uint16_t i = 0; i != strlen(argv[4]); i++) {
		if (!(isdigit(argv[4][i])) && argv[4][i] != '.') {
			fputs("\x1b[31mERROR\x1b[0m: fourth argument must be an integer or decimal number.", stderr);
			abort();
		}
	}
	
	retVal.confidence = atof(argv[4]);
	return retVal;
}