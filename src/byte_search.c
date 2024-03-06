#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "error_handling.h"

// Structure to store a directory entry
typedef struct {
    char *path;
} DirEntry;

// Function to search for a byte sequence in files
void search_byte_sequence(const char *directory_path, const char *target_search) {
    // Check if debugging is enabled
    int debug = 0;
    char *debug_env = getenv("LAB11DEBUG");
    if (debug_env != NULL && strcmp(debug_env, "1") == 0) {
        debug = 1;
    }

    if (strlen(target_search) < 4 || strncmp(target_search, "0x", 2) != 0) {
        handle_error("Invalid search format. Please provide a valid hexadecimal byte sequence (e.g., 0x1234).");
        return;
    }

    // Create stack for directory entries
    DirEntry *stack = malloc(sizeof(DirEntry));
    if (stack == NULL) {
        handle_error_with_exit("Memory allocation failed");
        return;
    }
    
    int stack_size = 1; // Current size of stack
    stack[0].path = strdup(directory_path);

    while (stack_size > 0) {
        // Pop directory from stack
        DirEntry current_dir = stack[--stack_size];

        DIR *dir = opendir(current_dir.path);
        if (dir == NULL) {
            // Print an error message if unable to open the directory
            handle_error("Unable to open directory");
            continue;
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            // Ignore "." and ".." directories
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }

            char *full_path = malloc(strlen(current_dir.path) + strlen(entry->d_name) + 2); // +2 for '/' and null terminator
            if (full_path == NULL) {
                handle_error_with_exit("Memory allocation failed");
                continue;
            }
            sprintf(full_path, "%s/%s", current_dir.path, entry->d_name);

            if (entry->d_type == DT_DIR) {
                // Push directory onto stack
                DirEntry *temp = realloc(stack, (stack_size + 1) * sizeof(DirEntry));
                if (temp == NULL) {
                    handle_error_with_exit("Memory allocation failed");
                    free(full_path);
                    continue;
                }
                stack = temp;
                stack[stack_size++].path = full_path;
            } else if (entry->d_type == DT_REG) {
                // If the entry is a regular file, search for the byte sequence
                FILE *file = fopen(full_path, "rb");
                if (file == NULL) {
                    // Print an error message if unable to open the file
                    handle_error("Unable to open file");
                    free(full_path);
                    continue;
                }

                int byte;
                int found = 0;
                int target = strtol(target_search, NULL, 0); // Convert the hex string to an integer
                int line_number = 1;

                while ((byte = fgetc(file)) != EOF) {
                    if (byte == target) {
                        found = 1;
                        if (debug) {
                            handle_debug("Byte sequence 0x%x found in file: %s at line number %d\n", target, full_path, line_number);
                        }
                        break;
                    }
                    line_number++;
                }
                if ((found == 0) && debug) {
                    handle_debug("Not found byte sequence 0x%x in filepath: %s\n", target, full_path);
                }

                if (found && !debug) {
                    printf("Byte sequence found in file: %s\n", full_path);
                } else if (!found && !debug) {
                    printf("Byte sequence not found in file: %s\n", full_path);
                }

                fclose(file);
                free(full_path);
            }
        }

        closedir(dir);
        free(current_dir.path);
    }

    // Free stack memory
    free(stack);
}
