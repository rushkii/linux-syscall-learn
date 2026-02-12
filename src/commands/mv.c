#include "../include/high.h"


int main(int argc, char *argv[]) {
	if (argc < 3) {
		println("Usage: ./mv <source> <destination>");
		return 1;
	}

	int moved = move(argv[1], argv[2]);

	if (moved < 0) println("Failed while trying to move the file");

	return 0;
}
