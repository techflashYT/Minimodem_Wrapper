#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define RESET "\e[0m"
#define YELLOW "\e[1;33m"
#define GREEN "\e[32m"
#define CYAN "\e[36m"
#define RED "\e[31m"

#define LOG_CODE \
	char tmp[64]; \
	snprintf(tmp, sizeof(tmp), "%s%s:%s%d", file, RESET, CYAN, line); \
	printf("%s[%s%-23s%s] %s%s%s\r\n", YELLOW, RESET GREEN, tmp, YELLOW, msgPre, msg, msgPost);
 
static void __attribute_maybe_unused__ _debug(char *file, int line, char *msg) {
	char *msgPre = RESET;
	char *msgPost = "";
	LOG_CODE
}

static void __attribute_maybe_unused__ _log(char *file, int line, char *msg) {
	char *msgPre = RESET;
	char *msgPost = "";
	LOG_CODE
}

static void __attribute_maybe_unused__ _warn(char *file, int line, char *msg) {
	char *msgPre = YELLOW;
	char *msgPost = RESET;
	LOG_CODE
}

static void __attribute_maybe_unused__ _error(char *file, int line, char *msg, bool fatal) {
	char *msgPre = RED "!!! ";
	char *msgPost = " !!!" RESET;
	LOG_CODE
	if (fatal) {
		exit(1);
	}
}

#ifdef ALLOW_DEBUG
#define debug(msg)        _debug((__FILE__ + 4), __LINE__, msg);
#else
#define debug(msg)        (void)0;
#endif


#define   log(msg)        _log  ((__FILE__ + 4), __LINE__, msg);
#define  warn(msg)        _warn ((__FILE__ + 4), __LINE__, msg);
#define error(msg, fatal) _error((__FILE__ + 4), __LINE__, msg, fatal);