#include <stdio.h>
#include <version.h>

void VER_Print() {
	printf("\e[1;33mTechflash \e[1;36mMinimodem Wrapper\e[0m - Version\e[0m\e[32m %s\e[0m\r\n", VER_String());
}