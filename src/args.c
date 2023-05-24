#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <minimodem.h>
options_t retVal;
static void usage(const char *progName) {
	fprintf(stderr,
		"USAGE:\r\n"
		"	`%s\' **file name for sending from/receiving to**  **`t\' for transmit or `r\' for receive**  **baud rate** **confidence value** **other various minimodem arguments**\r\n"
		"EXAMPLES:\r\n"
		"	To transmit `inputFile.bin\' at 300 baud with 2.75 confidence for a noisy cable: %s inputFile.bin t 300 2.75\r\n"
		"	To receive into `outputFile.bin\' at 1500 baud with 5 confidence for a very clean signal: %s outFile.bin r 1500 5\r\n",
		progName, progName, progName
	);
	exit(1);
}
options_t figureOutArgs(int argc, char *argv[]) {
	if (argc < 5) {
		usage(argv[0]);
	}
	memset(&retVal, '\0', sizeof(retVal));
	retVal.additionalArgs = malloc(2048);
	retVal.additionalArgs[0] = '\0';
	retVal.fileName = argv[1];
	if (argv[2][0] != 'r' && argv[2][0] != 't') {
		fprintf(stderr, "%sTransmit/Receive value must be exactly %s`%st%s\'%s or %s`%sr%s\'%s.%s\r\n", RED, RESET, B_CYAN, RESET, RED, RESET, B_CYAN, RESET, RED, RESET);
		exit(1);
	}
	if      (argv[2][0] == 'r') {retVal.mode = MODE_RECEIVE;}
	else if (argv[2][0] == 't') {retVal.mode = MODE_TRANSMIT;}
	for (uint_fast8_t i = 0; i != strlen(argv[3]); i++) {
		if (!(isdigit(argv[3][i])) && argv[3][i] != '.') {
			fprintf(stderr, "%sThird argument must be an %sinteger or decimal number%s.%s\r\n", RED, B_CYAN, RED, RESET);
			exit(1);
		}
	}
	retVal.baudRate   = atoi(argv[3]);
	for (uint_fast8_t i = 0; i != strlen(argv[4]); i++) {
		if (!(isdigit(argv[4][i])) && argv[4][i] != '.') {
			fprintf(stderr, "%sFourth argument must be an %sinteger or decimal number%s.%s\r\n", RED, B_CYAN, RED, RESET);
			exit(1);
		}
	}
	if (argc > 5) {
		fprintf(stderr, "%sWARNING: %sPassing all 5th and later args directly to minimodem%s\r\n", RED, B_YEL, RESET);
		for (uint8_t i = 0; i != (argc - 5); i++) {
			strncat(retVal.additionalArgs, argv[i + 5], (2048 - strlen(retVal.additionalArgs)));
		}
	}
	printf("Freeing %s%lu%s bytes from temporary buffer for additional arguments\r\n", B_CYAN, 2048 - (strlen(retVal.additionalArgs) + 1), RESET);
	retVal.additionalArgs = realloc(retVal.additionalArgs, strlen(retVal.additionalArgs) + 1);
	retVal.confidence = atof(argv[4]);
	return retVal;
}