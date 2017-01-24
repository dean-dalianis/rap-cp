#include <iostream>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "functions.hpp"

using namespace std;

int main(int argc, char **argv) {

	if (argc < 3)
		return -1;
	string origindir, destdir;
	bool verbose = false, links = false;
	for (int i = 0; i < argc - 1; i++) {
		if (!strcmp(argv[i], "-v"))
			verbose = true;
		else if (!strcmp(argv[i], "-l"))
			links = true;
		else
			origindir = argv[i];
	}
	destdir = argv[argc - 1];

	// Check if paths given are acceptable
	if (directories_check(origindir, destdir) == false) {
		return -1;
	}

	// Check if there is a file in destination catalog that does not exist in origin
	delete_different_files(origindir, destdir);
	// Traverse the origin directory until the end
	if (rapcp(origindir, destdir, verbose) == -1) {
		return -1;
	}

	return 0;
}
