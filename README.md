## Lab 1.1: File Byte Sequence Search

This lab project aims to develop a program for searching a specified byte sequence within files in a given directory. The program will traverse the directory recursively, searching for the specified byte sequence in each file it encounters. Upon finding the byte sequence, the program will provide information about the location of the sequence within the file.

### Features

- Search for a specified byte sequence within files in a directory.
- Recursively traverse subdirectories to search for files.
- Output information about the location of the byte sequence within each file.

### Usage

To use the program, follow these steps:

1. Compile the program using the provided Makefile:
    ```bash
    make all
    ```

2. Run the program with the specified byte sequence and directory path:
    ```bash
    ./lab11 <byte_sequence> <directory_path>
    ```

    Replace `<byte_sequence>` with the hexadecimal representation of the byte sequence you want to search for, and `<directory_path>` with the path to the directory where you want to start the search.

### Command Line Options

The program supports the following command line options:

- `-h`, `--help`: Output help information about the available options.
- `-v`, `--version`: Output the program version and information.

### Environment Variables

The program supports the following environment variable:

- `LAB11DEBUG`: Enable debugging information output.

### Implementation Details

The program is implemented in C and utilizes the standard C library functions for file operations and directory traversal. It follows POSIX standards and can be compiled and executed on Linux systems with a kernel version >= 5.0. The program adheres to the guidelines provided in the lab instructions, including error handling, proper memory management, and adherence to C11 standards.

### Directory Structure

The directory structure for the lab project is organized as follows:

- `src/`: Contains the source code files for the program.
- `include/`: Contains header files used in the program.
- `Makefile`: Automates the compilation process for the program.
- `README.md`: Provides information about the lab project.

### Contributing

If you'd like to contribute to this project, please fork the repository, make your changes, and submit a pull request. Your contributions are highly appreciated!

### Authors

- [Belyakov Nikita](https://github.com/17HIERARCH70)

### Contact

For inquiries or support, please contact [Belyakov Nikita](nikitabelekov@gmail.com).
