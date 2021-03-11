#ifndef _LOG_H_
#define _LOG_H_

#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>

#define INFO 1
#define ERROR 2


void log(int status, char *str, ...);

#endif