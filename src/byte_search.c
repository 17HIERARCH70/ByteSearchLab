#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "error_handling.h"

// Helper function to check if a path is a regular file
int is_regular_file(const char *path) {
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
        // Error occurred while getting file information
        return 0;
    }
    return S_ISREG(path_stat.st_mode);
}

// Function to search for a byte sequence in files
void search_byte_sequence(const char *directory_path, const char *target_search) {
    int debug=0;
    // Check if debugging is enabled
    char *debug_env = getenv("LAB11DEBUG");
    if (debug_env != NULL && strcmp(debug_env, "1") == 0) {
        debug=1;
    }
    
    if (strlen(target_search) < 4 || strncmp(target_search, "0x", 2) != 0) {
        handle_error("Invalid search format. Please provide a valid hexadecimal byte sequence (e.g., 0x1234).");
        return;
    }

    DIR *dir = opendir(directory_path);
    if (dir == NULL) {
        // Print an error message if unable to open the directory
        handle_error("Unable to open directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Ignore "." and ".." directories
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", directory_path, entry->d_name);

        if (is_regular_file(full_path)) {
            // If the entry is a regular file, search for the byte sequence
            FILE *file = fopen(full_path, "rb");
            if (file == NULL) {
                // Print an error message if unable to open the file
                handle_error("Unable to open file");
                continue;
            }

            int byte;
            int found = 0;
            int target = strtol(target_search, NULL, 0); // Convert the hex string to an integer
            int line_number = 1;

            while ((byte = fgetc(file)) != EOF) {
                if (byte == target) {
                    found = 1;
                    if (debug_env != NULL && strcmp(debug_env, "1") == 0) {
                        handle_debug("Byte sequence 0x%x found in file: %s at line number %d\n", target, full_path, line_number);
                    }
                    break;
                }
                line_number++;
            }
            if ((found==0) && debug==1) {
                handle_debug("Not found byte sequence 0x%x in filepath: %s\n", target, full_path);
            }

            if (found && debug!=1) {
                printf("Byte sequence found in file: %s\n", full_path);
            } else if (!found && debug!=1) {
                printf("Byte sequence not found in file: %s\n", full_path);
            }

            fclose(file);
        } else if (entry->d_type == DT_DIR) {
            // If the entry is a directory, recursively search it
            search_byte_sequence(full_path, target_search);
        }
    }

    closedir(dir);
}
