#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error_handling.h"

// Function to handle errors and output diagnostic messages
void handle_error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
}

// Function to handle debug messages
void handle_debug(const char *message) {
    char *debug_env = getenv("LAB11DEBUG");
    if (debug_env != NULL && strcmp(debug_env, "1") == 0) {
        fprintf(stderr, "Debug: %s\n", message);
    }
}