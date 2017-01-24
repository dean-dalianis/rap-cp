#include "functions.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <time.h>

using namespace std;



bool directories_check(string origindir, string destdir) {
	DIR *origin;
	DIR *destination;

// If origin directory does not exist return error
	if ((origin = open_directory(origindir)) == NULL) {
		cout << "error in opening origin directory: " << origindir << endl;
		return false;
	} else {
		if (closedir(origin) != 0)
			perror("error in closing origin directory");
	}

// If destination directory does not exist
	if ((destination = open_directory(destdir)) == NULL) {
		// Find position of last "/"
		unsigned position_of_last_col = destdir.find_last_of("/");
		// If position of last "/" was found save the path from the destdir beginning to this position
		if (position_of_last_col != string::npos) {
			string path = destdir.substr(0, position_of_last_col);
			struct stat sb;
			if (stat(path.c_str(), &sb) == 0 && (sb.st_mode & S_IFMT) == S_IFDIR) {
				// If the path exists create the directory
				if (mkdir(destdir.c_str(),
				S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0) {
					cout << "created directory " << destdir << endl;
					if ((destination = open_directory(destdir)) == NULL) {
						cout << "error in opening destination directory "
								<< destdir << endl;
						return false;
					} else {
						if (closedir(destination) != 0)
							perror("error in closing destination directory");
					}
				} else {
					cout << "error in creating destination directory "
							<< destdir << endl;
					return false;
				}
			} else {
				// If the path does not exist return error
				cout << "path: " << path << " does not exist." << endl;
				return false;
			}
		} else {
			// If no "/" was found create the directory
			if (mkdir(destdir.c_str(),
			S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0) {
				cout << "created directory " << destdir << endl;
				if ((destination == open_directory(destdir)) == 0) {
					cout << "error in opening destination directory " << destdir
							<< endl;
					return false;
				} else {
					if (closedir(destination) != 0)
						perror("error in closing destination directory");
				}
			} else {
				cout << "error in creating destination directory " << destdir
						<< endl;
				return false;
			}
		}
	}
	return true;
}

bool same_file(struct stat origin, struct stat destination) {
	if ((origin.st_size != destination.st_size)
			|| ((origin.st_size == destination.st_size)
					&& origin.st_mtime > destination.st_mtime))
		return false;
	return true;
}

bool same_dir(string origin, string destination) {
	DIR *orig = open_directory(origin);
	DIR *dest = open_directory(destination);
	if (orig == NULL) {
		perror("Error in opening origin directory");
		return false;
	}
	if (dest == NULL) {
		perror("Error in opening destination directory");
		return false;
	}
	struct dirent *direntp_orig;
	struct dirent *direntp_dest;
	struct stat statbuf_orig;
	struct stat statbuf_dest;
	string file_name_o, file_name_d;
	bool found;
	while ((direntp_orig = readdir(orig)) != NULL) {
		file_name_o = direntp_orig->d_name;
		found = false;
		while ((direntp_dest = readdir(dest)) != NULL) {
			file_name_d = direntp_dest->d_name;
			if (file_name_d == file_name_o) {
				found = true;
				break;
			}
		}
		if (found == false) {
			if (closedir(orig) != 0) {
				cout << "error in closing origin directory " << origin << endl;
				return false;
			}
			if (closedir(dest) != 0) {
				cout << "error in closing origin directory " << destination
						<< endl;
				return false;
			}
			return false;
		} else {
			string file_input_path = origin + "/" + file_name_o;
			string file_output_path = destination + "/" + file_name_o;
			if (stat(file_input_path.c_str(), &statbuf_orig) == -1) {
				perror("Failed to get file status");
				return false;
			}
			if (stat(file_output_path.c_str(), &statbuf_dest) == -1) {
				perror("Failed to get file status");
				return false;
			}
			if (((statbuf_orig.st_mode & S_IFMT)!= (statbuf_dest.st_mode & S_IFMT))
					|| (((statbuf_orig.st_mode & S_IFMT) == S_IFDIR)
							&& ((statbuf_dest.st_mode & S_IFMT) == S_IFDIR)
							&& !same_dir(file_input_path, file_output_path))
					|| (!((statbuf_orig.st_mode & S_IFMT) == S_IFDIR)
							&& !((statbuf_dest.st_mode & S_IFMT) == S_IFDIR)
							&& !same_file(statbuf_orig, statbuf_dest))) {
				if (closedir(orig) != 0) {
					cout << "error in closing origin directory " << origin
							<< endl;
					return false;
				}
				if (closedir(dest) != 0) {
					cout << "error in closing origin directory " << destination
							<< endl;
					return false;
				}
				return false;
			} else {
				if (closedir(orig) != 0) {
					cout << "error in closing origin directory " << origin
							<< endl;
					return false;
				}
				if (closedir(dest) != 0) {
					cout << "error in closing origin directory " << destination
							<< endl;
					return false;
				}
				return true;
			}
		}
	}
	while ((direntp_dest = readdir(dest)) != NULL) {
		file_name_d = direntp_dest->d_name;
		found = false;
		while ((direntp_orig = readdir(orig)) != NULL) {
			file_name_o = direntp_orig->d_name;
			if (file_name_o == file_name_d) {
				found = true;
				break;
			}
		}
		if (found == false) {
			if (closedir(orig) != 0) {
				cout << "error in closing origin directory " << origin << endl;
				return -1;
			}
			if (closedir(dest) != 0) {
				cout << "error in closing origin directory " << destination
						<< endl;
				return false;
			}
			return false;
		}
	}
	if (closedir(orig) != 0) {
		cout << "error in closing origin directory " << origin << endl;
		return false;
	}
	if (closedir(dest) != 0) {
		cout << "error in closing origin directory " << destination << endl;
		return false;
	}
	return true;
}

int copy_file(string input_path, string output_path, mode_t mode) {
	int output, input, fread;
	char buffer[1];
	if ((output = open(output_path.c_str(), O_CREAT | O_TRUNC | O_RDWR, mode))
			== -1) {
		perror("open");
		return -1;
	} else {
		if ((input = open(input_path.c_str(), O_RDONLY)) == -1) {
			close(output);
			perror("open");
			return -1;
		} else {
			while ((fread = read(input, buffer, sizeof(buffer))) > 0) {
				write(output, buffer, sizeof(buffer));
			}
			close(input);
		}
		close(output);
	}
	cout << input_path << endl;
	return 0;
}

int copy_directory(string input_path, string output_path, mode_t mode) {
	DIR *input_directory, *output_directory;
	string file_name_o;
	struct dirent *direntp_origin;
	struct stat statbuf_origin;
	mkdir(output_path.c_str(), mode);
	input_directory = open_directory(input_path);
	if (input_directory == NULL) {
		perror("Can not open directory");
		return -1;
	}
	output_directory = open_directory(output_path);
	if (output_directory == NULL) {
		perror("Can not open directory");
		if (closedir(input_directory) != 0) {
			cout << "error in closing origin directory " << input_path << endl;
			return -1;
		}
		return -1;
	}
	while ((direntp_origin = readdir(input_directory)) != NULL) {
		file_name_o = direntp_origin->d_name;
		if (file_name_o == "." || file_name_o == ".."
				|| file_name_o[file_name_o.length() - 1] == '~')
			continue;
		string new_input_path = input_path + "/" + file_name_o;
		if (stat(new_input_path.c_str(), &statbuf_origin) == -1) {
			perror("Failed to get file status");
			return -1;
		}
		string new_output_path = output_path + "/" + file_name_o;
		if ((statbuf_origin.st_mode & S_IFMT) == S_IFDIR) {
			// Input is directory
			if (copy_directory(new_input_path, new_output_path,
					statbuf_origin.st_mode) == -1)
				return -1;
		} else {
			// Input is file
			if (copy_file(new_input_path, new_output_path,
					statbuf_origin.st_mode) == -1)
				return -1;
		}
	}
	if (closedir(input_directory) != 0) {
		cout << "error in closing origin directory " << input_path << endl;
		return -1;
	}
	if (closedir(output_directory) != 0) {
		cout << "error in closing out directory " << output_path << endl;
		return -1;
	}
	return 0;
}

int rapcp(string input_path, string output_path, bool v) {
	DIR *input, *output;
	struct dirent *direntp_in;
	struct dirent *direntp_out;
	struct stat statbuf_origin;
	struct stat statbuf_destination;
	string file_name_o, file_name_d;
	bool found;
	string file_name;
	int count_files = 0;
	int count_copies = 0;
	int total_bytes = 0;
	time_t start, end;
	double elapsed;
	if ((input = open_directory(input_path)) == NULL) {
		perror("Cannot open directory");
		return -1;
	}
	if ((output = open_directory(output_path)) == NULL) {
		perror("Cannot open directory");
		return -1;
	}
	time(&start);
	while ((direntp_in = readdir(input)) != NULL) {
		file_name_o = direntp_in->d_name;
		if (file_name_o == "." || file_name_o == ".."
				|| file_name_o[file_name_o.length() - 1] == '~')
			continue;
		count_files++;&
		found = false;

		// Search the destination directory for the currently selected file of origin
		while ((direntp_out = readdir(output)) != NULL) {
			file_name_d = direntp_out->d_name;
			if (file_name_d == file_name_o) {
				found = true;
				break;
			}
		}
		rewinddir(output);
		if (found == false) {
			// If the file is not found create & copy (1)
			string file_input_path = input_path + "/" + file_name_o;
			if (stat(file_input_path.c_str(), &statbuf_origin) == -1) {
				perror("Failed to get file status");
				return -1;
			}
			if ((statbuf_origin.st_mode & S_IFMT) == S_IFDIR) {
				// Input file is a directory
				string file_output_path = output_path + "/" + file_name_o;
				if (copy_directory(file_input_path, file_output_path,
						statbuf_origin.st_mode) == -1)
					return -1;
				else {
					count_copies++;
					total_bytes += statbuf_origin.st_blksize;
				}
			} else {
				// Input file is a file
				string file_output_path = output_path + "/" + file_name_o;
				if (copy_file(file_input_path, file_output_path,
						statbuf_origin.st_mode) == -1)
					return -1;
				else {
					count_copies++;
					total_bytes += statbuf_origin.st_blksize;
				}
			}
		} else {
			// If the file is found check if it needs to be updated or not
			string file_input_path = input_path + "/" + file_name_o;
			string file_output_path = output_path + "/" + file_name_o;
			if (stat(file_input_path.c_str(), &statbuf_origin) == -1) {
				perror("Failed to get file status");
				return -1;
			}
			if (stat(file_output_path.c_str(), &statbuf_destination) == -1) {
				perror("Failed to get file status");
				return -1;
			}
			if (((statbuf_origin.st_mode & S_IFMT) != (statbuf_destination.st_mode & S_IFMT))
					|| (((statbuf_origin.st_mode & S_IFMT) == S_IFDIR)
							&& ((statbuf_destination.st_mode & S_IFMT)
									== S_IFDIR)
							&& !same_dir(file_input_path, file_output_path))
					|| (!((statbuf_origin.st_mode & S_IFMT) == S_IFDIR)
							&& !((statbuf_destination.st_mode & S_IFMT)
									== S_IFDIR)
							&& !same_file(statbuf_origin, statbuf_destination))) {
				// File needs to be updated (2)
				int temp;
				if ((temp = update(file_input_path, file_output_path, v))
						!= -1) {
					count_copies++;
					total_bytes += temp;
				} else
					return -1;
			} else {
				// File does not need to be updated (3)
			}
		}
	}
	time(&end);
	elapsed = difftime(end, start);
	if (closedir(input) != 0) {
		perror("Cannot close directory");
		return -1;
	}
	if (closedir(output) != 0) {
		perror("Cannot close directory");
		return -1;
	}
	if (v) {
		cout << "\nthere are " << count_files
				<< " files/directories in the hierarchy\n number of entities copied is "
				<< count_copies << "\n copied " << total_bytes << " bytes in "
				<< elapsed << "sec at " << total_bytes / elapsed << " bytes/sec"
				<< endl;
	}
	return 0;
}

int update(string input_path, string output_path, bool v) {
	struct stat statbuf_input;
	struct stat statbuf_output;
	int total_bytes = 0;
	if (stat(input_path.c_str(), &statbuf_input) == -1) {
		perror("Failed to get file status");
		return -1;
	}
	if (stat(output_path.c_str(), &statbuf_output) == -1) {
		perror("Failed to get file status");
		return -1;
	}
	if ((statbuf_input.st_mode & S_IFMT) == S_IFDIR) {
		// Input file is a directory
		total_bytes += statbuf_output.st_blksize;
		if ((statbuf_output.st_mode & S_IFMT) == S_IFDIR) {
			// Output file is a directory
			rapcp(input_path, output_path, false);
		} else {
			// Output file is a file
			delete_file(output_path);
			unsigned position_of_last_col = output_path.find_last_of("/");
			unsigned position_of_last_col_in = input_path.find_last_of("/");
			string path = output_path.substr(0, position_of_last_col) + "/" + input_path.substr(position_of_last_col_in);
			copy_directory(input_path, path, statbuf_input.st_mode);
		}
	} else {
		// Input file is a file
		total_bytes += statbuf_output.st_blksize;
		if ((statbuf_output.st_mode & S_IFMT) == S_IFDIR) {
			// Output file is a directory
			delete_file(output_path);
			unsigned position_of_last_col = output_path.find_last_of("/");
			unsigned position_of_last_col_in = input_path.find_last_of("/");
			string path = output_path.substr(0, position_of_last_col) + "/" + input_path.substr(position_of_last_col_in);
			copy_file(input_path, path, statbuf_input.st_mode);
		} else {
			// Output file is a file
			copy_file(input_path, output_path, statbuf_input.st_mode);
		}
	}
	return total_bytes;
}

int delete_directory(string directory) {
	DIR *dir;
	struct dirent *direntp;
	struct stat statbuf;
	string file_name;
	if ((dir = open_directory(directory)) == NULL) {
		perror("Can not open directory");
		return -1;
	}
	while ((direntp = readdir(dir)) != NULL) {
		file_name = direntp->d_name;
		if (file_name == "." || file_name == "..")
			continue;
		string new_file = directory + "/" + file_name;
		if (stat(new_file.c_str(), &statbuf) == -1) {
			perror("Failed to get file status");
			return -1;
		}
		if ((statbuf.st_mode & S_IFMT) == S_IFDIR) {
			// Input file is a directory
			delete_directory(new_file);
		} else {
			// Input file is a file
			remove(new_file.c_str());
		}
	}
	if (closedir(dir) != 0) {
		cout << "error in closing origin directory " << directory << endl;
		return -1;
	}
	remove(directory.c_str());
	return 0;
}

int delete_file(string file) {
	struct stat statbuf;
	if (stat(file.c_str(), &statbuf) == -1) {
		perror("Failed to get file status");
		return -1;
	}
	if ((statbuf.st_mode & S_IFMT) == S_IFDIR) {
		// Input file is a directory
		if (delete_directory(file) == -1)
			return -1;
	} else {
		// Input file is a file
		remove(file.c_str());
	}
	return 0;
}

int delete_different_files(string origind, string destdir) {
	DIR *origin, *destination;
	if ((origin = open_directory(origind)) == NULL) {
		return -1;
	}
	if ((destination = open_directory(destdir)) == NULL) {
		return -1;
	}
	struct dirent *direntp_origin;
	struct dirent *direntp_destination;
	string file_name_o, file_name_d;
	bool found;
	while ((direntp_destination = readdir(destination)) != NULL) {
		file_name_d = direntp_destination->d_name;
		if (file_name_d == "." || file_name_d == ".."
				|| file_name_d[file_name_d.length() - 1] == '~')
			continue;
		found = false;
		while ((direntp_origin = readdir(origin)) != NULL) {
			file_name_o = direntp_origin->d_name;
			if (file_name_o == file_name_d) {
				found = true;
				break;
			}
		}
		rewinddir(origin);
		if (found == false) {
			// If such a file is found, delete it (4)
			string dfile = destdir + "/" + file_name_d;
			if (delete_file(dfile) == -1)
				return -1;
		} else {
			struct stat statbuf;
			string dfile = destdir + "/" + file_name_d;
			if (stat(dfile.c_str(), &statbuf) == -1) {
				perror("failed to get file status");
				return -1;
			}
			if ((statbuf.st_mode & S_IFMT) == S_IFDIR) {
				string ofile = origind + "/" + file_name_d;
				delete_different_files(ofile, dfile);
			}
		}
	}
	rewinddir(origin);
	rewinddir(destination); 
	if (closedir(origin) != 0) {
		cout << "error in closing origin directory " << origin << endl;
		return -1;
	}
	if (closedir(destination) != 0) {
		cout << "error in closing destination directory " << destination << endl;
		return -1;
	}
	return 0;
}
