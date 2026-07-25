#include "common/log.h"

#define ANSI_RED "\033[31m"
#define ANSI_YELLOW "\033[33m"
#define ANSI_RESET "\033[0m"

void log_message(FILE *out, const char *prefix, const char *format, va_list args) {
    fprintf(out, "%s ", prefix);
    vfprintf(out, format, args);
    fputc('\n', out);
}

void log_info(FILE *out, const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_message(out, "[INFO]", format, args);
    va_end(args);
}

void log_warning(FILE *out, const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_message(out, ANSI_YELLOW "[WARN]" ANSI_RESET, format, args);
    va_end(args);
}

void log_error(FILE *out, const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_message(out, ANSI_RED "[ERROR]" ANSI_RESET, format, args);
    va_end(args);
}
