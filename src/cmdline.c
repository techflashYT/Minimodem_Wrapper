#include <stdio.h>
#include <string.h>
#include <log.h>
#include <stdbool.h>
#include <opts.h>

static int ARG_Mode = MODE_UNSET;
static void ARG_Parse(char *arg) {
	if (strcmp(arg, "-r") == 0 || strcmp(arg, "--rx") == 0 || strcmp(arg, "--receive") == 0) {
		if (ARG_Mode != MODE_UNSET) {
			error("You can't specify the mode twice!");
		}
		ARG_Mode = MODE_RX;
	}
	else if (strcmp(arg, "-t") == 0 || strcmp(arg, "--tx") == 0 || strcmp(arg, "--transmit") == 0) {
		if (ARG_Mode != MODE_UNSET) {
			error("You can't specify the mode twice!");
		}
		ARG_Mode = MODE_TX;
	}
	else {
		char *str = malloc(strlen(arg) + 32);
		sprintf(str, "Unknown argument: `%s'", arg); 
		error(str, true);
	}
	
}
int CMD_Parse(int argc, char *argv[]) {
	for (int i = 1; i != argc; i++) {
		ARG_Parse(argv[i]);
	}
	return 0;
}