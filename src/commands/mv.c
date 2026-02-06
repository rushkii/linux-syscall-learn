#include "./include/high.h"


int main(int argc, char *argv[]) {
	if (argc < 3) {
		println("Usage: ./mv <source> <destination>");
		return 1;
	}

	int moved = move(argv[1], argv[2]);

	return 0;
}

__attribute__((naked)) void _start(void) {
	// get the argc and argv from the rsp (register stack pointer)
	// argc were placed at 0
	// argv were placed at 8 bytes
	__asm__ volatile (
		".intel_syntax noprefix\n"
		"mov rdi, [rsp]\n"
		"lea rsi, [rsp + 8]\n"
		"and rsp, -16\n"

		"call main\n"

		"mov rdi, rax\n"
		"mov rax, 60\n"
		"syscall\n"

		".att_syntax\n"
		:
		:
		: "memory", "rdi", "rsi", "rax"
	);
}
