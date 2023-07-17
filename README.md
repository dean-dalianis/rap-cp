# Rapid Copy (rap-cp)

![GitHub top language](https://img.shields.io/github/languages/top/dean-dalianis/rap-cp?style=flat-square&color=5D6D7E)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/dean-dalianis/rap-cp?style=flat-square&color=5D6D7E)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

This is a command-line tool, implemented in C++, that performs rapid copying of files and directories from an origin directory to a destination directory. It ensures that the destination directory remains in sync with the origin directory, efficiently copying only the updated and missing files. The tool supports various options such as verbose mode and symbolic links preservation.

## Prerequisites

To compile and run the code, make sure you have the following:

- C++ compiler
- GNU Make

## Usage

The program takes command-line arguments to specify the origin directory, destination directory, and optional flags. The basic syntax is as follows:

```
./rapcp [options] <origin_directory> <destination_directory>
```

### Options

- `-v`: Verbose mode. Displays detailed information during the copying process.
- `-l`: Preserve symbolic links. Copies symbolic links as links instead of following them.

## Compilation

To compile the code using the provided Makefile, navigate to the project directory and run the following command:

```
make
```

This command will compile the code and generate the executable file named `rapcp`.

## Usage Examples

Here are some examples demonstrating the usage of the rapcp tool:

- Copying files and directories from the origin directory to the destination directory:
  ```
  ./rapcp <origin_directory> <destination_directory>
  ```

- Copying files and directories with verbose mode enabled:
  ```
  ./rapcp -v <origin_directory> <destination_directory>
  ```

- Copying files and directories while preserving symbolic links:
  ```
  ./rapcp -l <origin_directory> <destination_directory>
  ```
  
