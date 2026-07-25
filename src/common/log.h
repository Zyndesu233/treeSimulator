#ifndef TREE_SIMULATOR_LOG_H
#define TREE_SIMULATOR_LOG_H

#include <stdarg.h>
#include <stdio.h>

void log_info(FILE *out, const char *format, ...);
void log_warning(FILE *out, const char *format, ...);
void log_error(FILE *out, const char *format, ...);
void log_message(FILE *out, const char *prefix, const char *format, va_list args);

#endif
