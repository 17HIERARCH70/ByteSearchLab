#ifndef BYTE_SEARCH_H
#define BYTE_SEARCH_H

// Structure to store a directory entry
typedef struct {
    char *path;
} DirEntry;

// Function prototypes
int is_debug_enabled();
void push_directory_to_stack(DirEntry **stack, int *stack_size, const char *path);
void search_in_directory(const char *directory_path, const char *target_search, DirEntry **stack, int *stack_size);
void search_byte_sequence(const char *directory_path, const char *target_search);
void search_in_file(const char *file_path, const char *target_search);

#endif /* BYTE_SEARCH_H */
