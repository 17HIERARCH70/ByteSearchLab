#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "byte_search.h"
#include "error_handling.h"

// Function to display help information
void display_help() {
    printf("Usage: lab11 [OPTION]... TARGET_SEARCH DIRECTORY_PATH\n");
    printf("Searches for a byte sequence in files within the specified directory.\n\n");
    printf("Options:\n");
    printf("  -h, --help     display this help and exit\n");
    printf("  -v, --version  output version information and exit\n");
}

// Function to display version information
void display_version() {
    printf("Lab1.1\n");
    printf("Group Number: 3245\n");
    printf("Variant Number: 1 (even)\n");
}

int main(int argc, char *argv[]) {
    // Check if debugging is enabled
    char *debug_env = getenv("LAB11DEBUG");
    if (debug_env != NULL && strcmp(debug_env, "1") == 0) {
        printf("\033[1;32mDebugging is enabled.\033[0m \n\n");
        printf("\033[1;32mList of args:\033[0m \n");
        printf("argc = %d\n", argc);
        for (int i = 0; i < argc; i++) {
            printf("argv[%d] = %s\n", i, argv[i]);
        }
    }

    // Check if no arguments are provided
    if (argc < 2) {
        handle_error("No arguments provided.");
        display_help();
        return 1;
    }

    // Check for help flag
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        display_help();
        return 0;
    }

    // Check for version flag
    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
        display_version();
        return 0;
    }

    // Check if invalid option is provided
    if (argv[1][0] == '-') {
        handle_error("Invalid option provided.");
        display_help();
        return 1;
    }

    // Check for correct number of arguments
    if (argc != 3) {
        handle_error("Incorrect number of arguments.");
        display_help();
        return 1;
    }

    // Perform byte sequence search
    search_byte_sequence(argv[2], argv[1]);
    
    return 0;
}
