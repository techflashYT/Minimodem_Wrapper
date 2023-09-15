#include <version.h>
#include <cmdline.h>
#include <opts.h>
#include <log.h>
extern void SRV_Start();
extern void CL_Start();
int main(int argc, char *argv[]) {
	VER_Print();
	CMD_Parse(argc, argv);
	debug("Command line parsed, entering client/server main loop");
	if (opts.mode == MODE_RX) {
		debug("Starting server");
		SRV_Start();
	}
	else {
		debug("Starting client");
		CL_Start();
	}

}