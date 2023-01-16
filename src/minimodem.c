#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <minimodem.h>
#include <errno.h>
#include <sys/stat.h>
char *tmpDataFile;
char *stderrLog;
char *tmpScriptName;
bool minimodem(uint8_t *file, size_t size, bool mode, uint16_t baudRate, double confidence) {
	char *command = malloc(300 + size);
	char *buffer  = malloc(size);
	char *confBuf = malloc(10);
	char *pipeStr = malloc(128);

	if (buffer == NULL || command == NULL || confBuf == NULL || pipeStr == NULL) {
		fprintf(stderr, "%smalloc() call failed!%s.  Are you out of memory?\r\n", RED, RESET);
		abort();
	}

	tmpDataFile   = malloc(34);
	stderrLog     = malloc(34);
	tmpScriptName = malloc(34);
	strcpy(tmpDataFile, "/tmp/TechflashMinimodemTmp_XXXXXX");
	strcpy(stderrLog, "/tmp/TechflashMinimodemTmp_XXXXXX");
	strcpy(tmpScriptName, "/tmp/TechflashMinimodemTmp_XXXXXX");
	// we close those 2 instantly, since we don't need them open, the shell script will be using them.
	close(mkstemp(tmpDataFile));
	close(mkstemp(stderrLog));

	errno = 0; // reset errno in case it got messed up by something else
	FILE *fp = fdopen(mkstemp(tmpScriptName), "w+");
	char modeChar = '\0';
	if (fp == 0 || errno != 0) {
		fprintf(stderr, "%sFailed to open temporary file!%s\r\nDebug Info:\r\n- Return Value: %p\r\n", RED, RESET, fp);
		perror("- perror()");
		remove(stderrLog);
		remove(tmpDataFile);
		remove(tmpScriptName);
		abort();
	}

	if      (mode == MODE_RECEIVE)  {modeChar = 'r';}
	else if (mode == MODE_TRANSMIT) {modeChar = 't';}

	// if we're receiving, don't send anything to minimodem's stdin.
	pipeStr[0] = '\0';
	if (mode == MODE_TRANSMIT) {sprintf(pipeStr, "cat %s | ", (char *)file);}
	fprintf(fp, "#!/bin/bash\n%sminimodem -%c %u -c %.3f --rx-one 2> >(tee %s >&2) > %s\n", pipeStr, modeChar, baudRate, confidence, stderrLog, tmpDataFile);

	free(confBuf);

	chmod(tmpScriptName, 0777);

	errno = 0; // reset errno in case it got messed up by something else
	int ret = fclose(fp);
	if (ret != 0 || errno != 0) {
		fprintf(stderr, "%sFailed to close temporary file!%s\r\nDebug Info:\r\n- Return Value: %d\r\n", RED, RESET, ret);
		perror("- perror()");
		remove(stderrLog);
		remove(tmpDataFile);
		remove(tmpScriptName);
		abort();
	}

	ret = system(tmpScriptName);
	int errnoBak = errno;
	if (ret != 0) {
		fprintf(stderr, "%sError running minimodem!%s  Perhaps it's not installed?\r\nDebug Info:\r\n- Return Value: %u\r\n", RED, RESET, (uint8_t)ret);
		errno = errnoBak;
		perror("- perror()");
		remove(stderrLog);
		remove(tmpDataFile);
		remove(tmpScriptName);
		abort();
	}

	// clean up temp variables
	free(command);
	free(buffer);
	free(pipeStr);
	
	// were we trying to read data?  if so, `file` is a buffer, and `size` is it's size
	if (mode == MODE_RECEIVE) {
		fp = fopen(tmpDataFile, "r");
		// get file size
		fseek(fp, 0, SEEK_END);
		size_t sizeOfFile = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		memset(file, '\0', size);
		if (sizeOfFile > size) {
			// size of the file is bigger than expected!  abort before we blow past the end of the buffer
			fprintf(stderr, "%sUh oh!  We received %s%lu%s more bytes than we have room for!  Aborting before bad things happen!%s\r\n", RED, B_CYAN, sizeOfFile - size, RED, RESET);
			abort();
		}
		fread(file, size, 1, fp);
		fclose(fp);
	}

	remove(stderrLog);
	free(stderrLog);
	remove(tmpDataFile);
	free(tmpDataFile);
	remove(tmpScriptName);
	free(tmpScriptName);
	return true;
}