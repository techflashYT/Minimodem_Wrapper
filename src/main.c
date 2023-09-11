#include <version.h>
#include <cmdline.h>
#include <log.h>
int main(int argc, char *argv[]) {
	VER_Print();
	CMD_Parse(argc, argv);
	debug("hello world");
	
}