#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <getopt.h>  
#include "byte_search.h"
#include "error_handling.h"

#define HELP_OPTION 'h'
#define VERSION_OPTION 'v'

void display_help() {
    printf("Usage: lab11 [OPTION]... TARGET_SEARCH DIRECTORY_PATH\n");
    printf("Searches for a byte sequence in files within the specified directory.\n\n");
    printf("Options:\n");
    printf("  -%c, --help     display this help and exit\n", HELP_OPTION);
    printf("  -%c, --version  output version information and exit\n", VERSION_OPTION);
}

void display_version() {
    printf("Lab1.1\n");
    printf("Group Number: 3245\n");
    printf("Variant Number: 1 (even)\n");
}

void handle_invalid_option() {
    handle_error("Invalid option provided.");
    display_help();
}

int main(int argc, char *argv[]) {
    int opt;
    struct option long_options[] = {
        {"help", no_argument, 0, HELP_OPTION},
        {"version", no_argument, 0, VERSION_OPTION},
        {0, 0, 0, 0} // Завершающая строка
    };

    while ((opt = getopt_long(argc, argv, "hv", long_options, NULL)) != -1) {
        switch (opt) {
            case HELP_OPTION:
                display_help();
                return 0;
            case VERSION_OPTION:
                display_version();
                return 0;
            default:
                handle_invalid_option();
                return -1;
        }
    }

    if (argc - optind != 2) {
        handle_error("Incorrect number of arguments.");
        display_help();
        return -1;
    }

    char *target_search = argv[optind];
    char *directory_path = argv[optind + 1];

    // Perform byte sequence search
    search_byte_sequence(directory_path, target_search);
    
    return 0;
}
