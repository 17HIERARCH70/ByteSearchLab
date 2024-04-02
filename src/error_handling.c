#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "error_handling.h"

// Function to handle errors and output diagnostic messages
void handle_error(const char *message) {
    fprintf(stderr, "\033[1;31mError:\033[0m %s\n", message);
}

void handle_error_with_exit(const char *message) {
    fprintf(stderr, "\033[1;31mError:\033[0m %s\n", message);
    exit(EXIT_FAILURE); 
}

// Function to handle debug messages
void handle_debug(const char *format, ...) {
    char *debug_env = getenv("LAB11DEBUG");
    if (debug_env != NULL && strcmp(debug_env, "1") == 0) {
        va_list args;
        va_start(args, format);
        fprintf(stderr, "\033[1;32mDebug:\033[0m "); 
        vfprintf(stderr, format, args);
        va_end(args);
    }
}
