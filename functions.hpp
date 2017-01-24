#ifndef FUNCTIONS_HPP_
#define FUNCTIONS_HPP_

#include <sys/types.h>
#include <dirent.h>
#include <string>

DIR* open_directory(std::string);	// Opens a directory

bool directories_check(std::string, std::string);	// Check if given directories are acceptable

bool same_file(struct stat, struct stat);	// Checks if 2 files are the same using the inodes

bool same_dir(std::string, std::string);	// Checks if 2 directories are the same

int copy_file(std::string, std::string, mode_t);	// Copies a files

int copy_directory(std::string, std::string, mode_t);	// Copies a directory

int rapcp(std::string, std::string, bool);	// Rapid Copy

int update(std::string, std::string, bool v);	// Updates an folder that already exists

int delete_directory(std::string);	// Deletes a directory and all of its contents

int delete_file(std::string);	// Deletes a file

int delete_different_files(std::string, std::string);	// Deletes files that exist in destination folder but not in origin folder

#endif /* FUNCTIONS_HPP_ */
