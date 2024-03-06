#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

// Function to handle errors and output diagnostic messages
void handle_error(const char *message);
void handle_error_with_exit(const char *message) ;

// Function to handle debug messages
void handle_debug(const char *format, ...);

#endif /* ERROR_HANDLING_H */
