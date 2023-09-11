#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
static void __attribute_maybe_unused__ _log(char *file, int line, char *msg) {
	printf("\e[1;33m[\e[0m\e[32m%s\e[0m:\e[36m%d\e[1;33m]\e[0m %s\r\n", file, line, msg);	
}

static void __attribute_maybe_unused__ _warn(char *file, int line, char *msg) {
	printf("\e[1;33m[\e[0m\e[32m%s\e[0m:\e[36m%d\e[1;33m]\e[0m %s\r\n", file, line, msg);	
}

static void __attribute_maybe_unused__ _error(char *file, int line, char *msg, bool fatal) {
	printf("\e[1;33m[\e[0m\e[32m%s\e[0m:\e[36m%d\e[1;33m]\e[31m !!!\e[0\e[31m %s \e[1m!!!\e[0m\r\n", file, line, msg);
	if (fatal) {
		exit(1);
	}
}


#define   log(msg)        _log  ((__FILE__ + 4), __LINE__, msg);
#define  warn(msg)        _warn ((__FILE__ + 4), __LINE__, msg);
#define error(msg, fatal) _error((__FILE__ + 4), __LINE__, msg, fatal);