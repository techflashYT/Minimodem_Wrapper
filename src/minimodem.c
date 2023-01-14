#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <minimodem.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
bool minimodem(uint8_t *data, size_t size, bool mode, uint16_t baudRate, double confidence) {
	char *command = malloc(300 + size);
	if (command == NULL) {
		fprintf(stderr, "\x1b[31mmalloc() call failed to allocate memory for file\x1b[0m.  Are you out of memory?\r\n");
		abort();
	}
	char *buffer  = malloc(size);
	if (buffer == NULL) {
		fprintf(stderr, "\x1b[31mmalloc() call failed to allocate memory for file\x1b[0m.  Are you out of memory?\r\n");
		abort();
	}
	strcpy(command, "cat << THIS_LINE_RIGHT_HERE_THAT_NOTHING_ELSE_WILL_EVER_USE_IS_THE_END_OF_THE_FILE | minimodem -");
	if (mode == MODE_RECEIVE) {
		strcpy(command + strlen(command), "r ");
	}
	else if (mode == MODE_TRANSMIT) {
		strcpy(command + strlen(command), "t ");
	}
	sprintf(command + strlen(command), "%u", baudRate);
	strcpy(command + strlen(command), " -c ");
	char *confBuf = malloc(10);
	if (confBuf == NULL) {
		fprintf(stderr, "\x1b[31mmalloc() call failed to allocate memory for file\x1b[0m.  Are you out of memory?\r\n");
		abort();
	}
	sprintf(confBuf, "%.3f", confidence);
	strcpy(command + strlen(command), confBuf);
	free(confBuf);

	char stdoutLog[] = "/tmp/TechflashMinimodemTmp_XXXXXX";
	mkstemp(stdoutLog);
	char stderrLog[] = "/tmp/TechflashMinimodemTmp_XXXXXX";
	mkstemp(stderrLog);
	char tmpScriptName[] = "/tmp/TechflashMinimodemTmp_XXXXXX";
	mkstemp(tmpScriptName);
	strcpy(command + strlen(command), " 2> >(tee ");
	strcpy(command + strlen(command), stderrLog);
	strcpy(command + strlen(command), " >&2) > ");
	strcpy(command + strlen(command), stdoutLog);
	strcpy(command + strlen(command), "\r\n");
	size_t curSize = strlen(command);
	memcpy(command + curSize, data, size);
	command[curSize + size] = '\0';
	strcpy(command + strlen(command), "\r\nTHIS_LINE_RIGHT_HERE_THAT_NOTHING_ELSE_WILL_EVER_USE_IS_THE_END_OF_THE_FILE\r\n");

	chmod(tmpScriptName, 0777);
	errno = 0; // reset errno in case it got messed up by something else
	FILE *fp = fopen(tmpScriptName, "w+");
	if (fp == 0 || errno != 0) {
		fprintf(stderr, "\x1b[31mFailed to open temporary file!\x1b[0m\r\nDebug Info:\r\n- Return Value: %p\r\n", fp);
		perror("- perror()");
		remove(stderrLog);
		remove(stdoutLog);
		// remove(tmpScriptName);
		abort();
	}
	errno = 0; // reset errno in case it got messed up by something else
	int ret = fwrite(command, 300 + size, 1, fp);
	if (ret != 1 || errno != 0) {
		fprintf(stderr, "\x1b[31mFailed to write temporary file!\x1b[0m\r\nDebug Info:\r\n- Return Value: %d\r\n", ret);
		perror("- perror()");
		remove(stderrLog);
		remove(stdoutLog);
		remove(tmpScriptName);
		abort();
	}
	ret = fclose(fp);
	if (ret != 0 || errno != 0) {
		fprintf(stderr, "\x1b[31mFailed to close temporary file!\x1b[0m\r\nDebug Info:\r\n- Return Value: %d\r\n", ret);
		perror("- perror()");
		remove(stderrLog);
		remove(stdoutLog);
		remove(tmpScriptName);
		abort();
	}

	ret = system(tmpScriptName);
	sleep(50000);
	int errnoBak = errno;
	if (ret != 0) {
		fprintf(stderr, "\x1b[31mError running minimodem!\x1b[0m  Perhaps it's not installed?\r\nDebug Info:\r\n- Return Value: %u\r\n", (uint8_t)ret);
		errno = errnoBak;
		perror("- perror()");
		remove(stderrLog);
		remove(stdoutLog);
		remove(tmpScriptName);
		abort();
	}
	FILE *fd = fopen(stderrLog, "r");
	fread(buffer, size, 1, fd);
	free(command);
	free(buffer);
	remove(stderrLog);
	remove(stdoutLog);
	remove(tmpScriptName);
}