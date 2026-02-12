#include "../include/high.h"


int main(int argc, char *argv[]) {
	if (argc < 2) {
		println("Usage: ./cat <source> <destination>");
		return 1;
	}

	int reads = fread(argv[1]);

	if (reads < 0) println("Failed while trying to read the file");

	return 0;
}
