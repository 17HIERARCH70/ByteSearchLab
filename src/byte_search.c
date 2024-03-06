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

// Function to check if debugging is enabled
int is_debug_enabled() {
    int debug = 0;
    char *debug_env = getenv("LAB11DEBUG");
    if (debug_env != NULL && strcmp(debug_env, "1") == 0) {
        debug = 1;
    }
    return debug;
}

// Function to push directory onto stack
void push_directory_to_stack(DirEntry **stack, int *stack_size, const char *path) {
    DirEntry *temp = realloc(*stack, (*stack_size + 1) * sizeof(DirEntry));
    if (temp == NULL) {
        handle_error_with_exit("Memory allocation failed");
    }
    *stack = temp;
    (*stack)[(*stack_size)++].path = strdup(path);
}

// Function to search in directory
void search_in_directory(const char *directory_path, const char *target_search, DirEntry **stack, int *stack_size) {
    DIR *dir = opendir(directory_path);
    if (dir == NULL) {
        handle_error("Unable to open directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Ignore "." and ".." directories
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char *full_path = malloc(strlen(directory_path) + strlen(entry->d_name) + 2); // +2 for '/' and null terminator
        if (full_path == NULL) {
            handle_error_with_exit("Memory allocation failed");
        }
        sprintf(full_path, "%s/%s", directory_path, entry->d_name);

        if (entry->d_type == DT_DIR) {
            // Push directory onto stack
            push_directory_to_stack(stack, stack_size, full_path);
        } else if (entry->d_type == DT_REG) {
            // If the entry is a regular file, search for the byte sequence
            search_in_file(full_path, target_search);
        }
        free(full_path);
    }
    closedir(dir);
}

// Function to search in file
void search_in_file(const char *file_path, const char *target_search) {
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        handle_error("Unable to open file");
        return;
    }

    int target = strtol(target_search, NULL, 0); // Convert the hex string to an integer
    int debug = is_debug_enabled();
    int byte;
    int found = 0;
    int line_number = 1;

    while ((byte = fgetc(file)) != EOF) {
        if (byte == target) {
            found = 1;
            if (debug) {
                handle_debug("Byte sequence 0x%x found in file: %s at line number %d\n", target, file_path, line_number);
            }
            break;
        }
        line_number++;
    }
    if ((found == 0) && debug) {
        handle_debug("Not found byte sequence 0x%x in filepath: %s\n", target, file_path);
    }

    if (found && !debug) {
        printf("Byte sequence found in file: %s\n", file_path);
    } else if (!found && !debug) {
        printf("Byte sequence not found in file: %s\n", file_path);
    }

    fclose(file);
}

// Main search function
void search_byte_sequence(const char *directory_path, const char *target_search) {
    if (strlen(target_search) < 4 || strncmp(target_search, "0x", 2) != 0) {
        handle_error("Invalid search format. Please provide a valid hexadecimal byte sequence (e.g., 0x1234).");
        return;
    }

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
        search_in_directory(current_dir.path, target_search, &stack, &stack_size);
        free(current_dir.path);
    }
    
    free(stack);
}
