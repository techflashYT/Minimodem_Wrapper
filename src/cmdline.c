#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <log.h>
#include <opts.h>

opts_t opts;
bool gotBaudRate;

#define argIs(x) strcmp(arg, x) == 0
static void (*nextArgHandler)(char *arg) = NULL;

static void ARG_ParseBaudRate(char *arg) {
	char *validTest = NULL;
	int64_t result = strtoq(arg, &validTest, 0);
	if (validTest == NULL || *validTest != '\0' || result == LONG_MAX || result == LONG_MIN || result == 0) {
		#ifdef ALLOW_DEBUG
			char *reason = "???";
			if (validTest == NULL)  {reason = "validTest == NULL";}
			else if (*validTest != '\0') {reason = "*validTest != '\\0'";}
			else if (result == LONG_MAX) {reason = "result == LONG_MAX";}
			else if (result == LONG_MIN) {reason = "result == LONG_MIN";}
			else if (result == 0)        {reason = "result == 0";}
			char tmp[64];
			sprintf(tmp, "Reason: %s", reason);
			debug(tmp);
		#endif
		error("Invalid baud rate specified", true);
	}
	debug("Found valid baud rate");
	opts.baudRate = result;
}

static void ARG_Parse(char *arg) {
	if (nextArgHandler != NULL) {
		nextArgHandler(arg);
		nextArgHandler = NULL;
		return;
	}
	if (argIs("-r") || argIs("--rx") || argIs("--receive")) {
		if (opts.mode != MODE_UNSET) {
			error("You can't specify the mode twice!", true);
		}
		opts.mode = MODE_RX;
	}
	else if (argIs("-t") || argIs("--tx") || argIs("--transmit")) {
		if (opts.mode != MODE_UNSET) {
			error("You can't specify the mode twice!", true);
		}
		opts.mode = MODE_TX;
	}
	else if (argIs("-b") || argIs("--baud") || argIs("--rate") || argIs("--speed")) {
		if (gotBaudRate) {
			error("You can't specify the baud rate twice!", true);
		}
		gotBaudRate = true;
		nextArgHandler = ARG_ParseBaudRate;
	}
	else {
		char *str = malloc(strlen(arg) + 32);
		sprintf(str, "Unknown argument: `%s'", arg); 
		error(str, true);
	}
	
}
void CMD_Parse(int argc, char *argv[]) {
	// set up default values
	opts.mode = MODE_UNSET;
	opts.baudRate = 0;

	for (int i = 1; i != argc; i++) {
		ARG_Parse(argv[i]);
	}
	#ifdef ALLOW_DEBUG
		char tmp[64];
		char *modestr;
		bool err = false;
		
		debug("Options:");
		
		if (opts.mode == MODE_RX) {modestr = "Receive";}
		else if (opts.mode == MODE_TX) {modestr = "Transmit";}
		else if (opts.mode == MODE_UNSET) {modestr = "Unset"; err = true;}
		else {modestr = "!!! Invalid value !!!"; err = true;}
		snprintf(tmp, sizeof(tmp), "  - Mode: %s", modestr);

		if (err) {
			error(tmp, false);
		}
		else {
			debug(tmp);
		}
		snprintf(tmp, sizeof(tmp), "  - Baud Rate: %u", opts.baudRate);
		debug(tmp);
	#endif
	// all options parsed, check for anything unset
	if (opts.mode == MODE_UNSET) {
		error("You must specify a mode", true);
	}
	if (opts.baudRate == 0) {
		error("You must specify a baud rate", true);
	}
	return;
}