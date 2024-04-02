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

// Convert hex string to byte array
unsigned char *hex_string_to_byte_array(const char *hex_string, int *length) {
    size_t hex_length = strlen(hex_string);
    if (hex_length < 3 || strncmp(hex_string, "0x", 2) != 0) {
        *length = 0;
        return NULL;
    }
    hex_length -= 2; // without "0x"
    *length = hex_length / 2;
    unsigned char *bytes = (unsigned char *)malloc(*length);
    if (bytes == NULL) {
        handle_error_with_exit("Memory allocation failed");
    }

    for (int i = 0; i < *length; i++) {
        sscanf(&hex_string[2 + i * 2], "%2hhx", &bytes[i]);
    }

    return bytes;
}

// Function to push directory onto stack
void push_directory_to_stack(DirEntry **stack, int *stack_size, const char *path) {
    DirEntry *temp = (DirEntry *)realloc(*stack, (*stack_size + 1) * sizeof(DirEntry));
    if (temp == NULL) {
        handle_error_with_exit("Memory allocation failed");
    }
    *stack = temp;
    (*stack)[(*stack_size)].path = strdup(path);
    (*stack_size)++;
}

// Function to search in file for a sequence of bytes
void search_in_file(const char *file_path, const unsigned char *target_search, int search_length) {
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        handle_error("Unable to open file");
        return;
    }

    int debug = is_debug_enabled();
    unsigned char *buffer = (unsigned char *)malloc(search_length);
    if (buffer == NULL) {
        handle_error_with_exit("Memory allocation failed");
    }

    int found = 0;
    size_t bytesRead;
    int offset = 0;
    while ((bytesRead = fread(buffer, 1, search_length, file)) == (size_t)search_length) {
        if (memcmp(buffer, target_search, search_length) == 0) {
            found = 1;
            if (debug) {
                handle_debug("Byte sequence found in file: %s, at %d bytes offset\n", file_path, offset);
            } else {
                printf("Byte sequence found in file: %s\n", file_path);
            }
            break;
        }
        fseek(file, ++offset, SEEK_SET); 
    }

    if (!found && debug) {
        handle_debug("Byte sequence not found in file: %s\n", file_path);
    }

    free(buffer);
    fclose(file);
}

// Function to search in directory with byte sequence
void search_in_directory(const char *directory_path, const unsigned char *target_search, int search_length, DirEntry **stack, int *stack_size) {
    DIR *dir = opendir(directory_path);
    if (dir == NULL) {
        handle_error("Unable to open directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char *full_path;
        if (directory_path[strlen(directory_path) - 1] == '/') {
            full_path = (char *)malloc(strlen(directory_path) + strlen(entry->d_name) + 1);
            sprintf(full_path, "%s%s", directory_path, entry->d_name);
        } else {
            full_path = (char *)malloc(strlen(directory_path) + strlen(entry->d_name) + 2);
            sprintf(full_path, "%s/%s", directory_path, entry->d_name);
        }

        if (full_path == NULL) {
            handle_error_with_exit("Memory allocation failed");
        }

        if (entry->d_type == DT_DIR) {
            push_directory_to_stack(stack, stack_size, full_path);
        } else if (entry->d_type == DT_REG) {
            search_in_file(full_path, target_search, search_length);
        }
        free(full_path);
    }
    closedir(dir);
}


// Function to start searching for a byte sequence in a directory
void search_byte_sequence(const char *directory_path, const char *hex_search) {
    int search_length;
    unsigned char *target_search = hex_string_to_byte_array(hex_search, &search_length);
    if (target_search == NULL) {
        handle_error("Invalid search format or memory allocation failed");
        return;
    }

    DirEntry *stack = NULL;
    int stack_size = 0;
    push_directory_to_stack(&stack, &stack_size, directory_path);

    while (stack_size > 0) {
        DirEntry current_dir = stack[--stack_size];
        search_in_directory(current_dir.path, target_search, search_length, &stack, &stack_size);
        free(current_dir.path);
    }
    
    free(stack);
    free(target_search);
}