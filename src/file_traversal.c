#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "error_handling.h"

// Function to handle traversal of directories
void traverse_directory_recursive(const char *path);

// Helper function to check if a path is a directory
int is_directory(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

// Function to traverse a directory recursively
void traverse_directory(const char *path) {
    if (!is_directory(path)) {
        // Print an error message if the provided path is not a directory
        handle_error("Provided path is not a directory");
        return;
    }
    traverse_directory_recursive(path);
}

// Recursive function to traverse a directory
void traverse_directory_recursive(const char *path) {
    // Check if debugging is enabled
    char *debug_env = getenv("LAB11DEBUG");
    if (debug_env != NULL && strcmp(debug_env, "1") == 0) {
        handle_debug("Debugging enabled in file traversal.");
    }

    DIR *dir = opendir(path);
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
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        // Print the full path of each file/directory
        printf("%s\n", full_path);

        if (is_directory(full_path)) {
            // If the entry is a directory, recursively traverse it
            traverse_directory_recursive(full_path);
        }
    }

    closedir(dir);
}
