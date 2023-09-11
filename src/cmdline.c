#include <stdio.h>
#include <string.h>
#include <log.h>
#include <stdbool.h>
#include <opts.h>

static int ARG_Mode = MODE_UNSET;
static void ARG_Parse(char *arg) {
	if (strcmp(arg, "-r") == 0 || strcmp(arg, "--rx") == 0 || strcmp(arg, "--receive") == 0) {
		if (ARG_Mode != MODE_UNSET) {
			error("You can't specify the mode twice!", true);
		}
		ARG_Mode = MODE_RX;
	}
	else if (strcmp(arg, "-t") == 0 || strcmp(arg, "--tx") == 0 || strcmp(arg, "--transmit") == 0) {
		if (ARG_Mode != MODE_UNSET) {
			error("You can't specify the mode twice!", true);
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
	#ifdef ALLOW_DEBUG
	char tmp[64];
	char *modestr;
	bool err = false;
	
	debug("Options:");
	
	if (ARG_Mode == MODE_RX) {modestr = "Receive";}
	else if (ARG_Mode == MODE_TX) {modestr = "Transmit";}
	else if (ARG_Mode == MODE_UNSET) {modestr = "!!! Unset !!!"; err = true;}
	else {modestr = "!!! Invalid value !!!"; err = true;}
	snprintf(tmp, sizeof(tmp), "  - Mode: %s", modestr);

	if (err) {
		error(tmp, false);
	}
	else {
		debug(tmp);
	}

	#endif
	// all options parsed, check for anything unset
	if (ARG_Mode == MODE_UNSET) {
		error("You must specify a mode", true);
	}
	return 0;
}