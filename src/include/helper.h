#ifndef SYSCALL_H
#define SYSCALL_H

/*
* `=a` : means write rax
* `a`  : means read rax
* `+a` : means read/write rax

* what is `rax` mean, anyway?
* `rax` is a register often used for syscall code
* e.g: `mov rax, 60` means calling a linux exit() syscall
* 60 is the syscall code for the exit().
* Refer: https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md

* what about D, S, d and r?
* well, D is rdi. so, what's difference between D and d?
* D is rdi, and d is rdx. so, what are S and r?
* s is rsi, and r is register like r10, r12, and so on. how do I write r10 and r12?
* you can define it first with other inline assembly register
* e.g: `register long reg10 __asm__("r10") = r10;`
* and then pass reg10 to the `"r", (reg10)`
*/

static inline long syscall0(long rax) {
	__asm__ volatile (
		"syscall"
		: "+a" (rax)
		:
		: "rcx", "r11", "memory"
	);
	return rax;
}

static inline long syscall1(long rax, long rdi) {
	__asm__ volatile (
		"syscall"
		: "+a" (rax)
		: "D" (rdi)
		: "rcx", "r11", "memory"
	);
	return rax;
}

static inline long syscall2(long rax, long rdi, long rsi) {
	__asm__ volatile (
		"syscall"
		: "+a" (rax)
		: "D" (rdi), "S" (rsi)
		: "rcx", "r11", "memory"
	);
	return rax;
}

static inline long syscall3(long rax, long rdi, long rsi, long rdx) {
	__asm__ volatile (
		"syscall"
		: "+a" (rax)
		: "D" (rdi), "S" (rsi), "d" (rdx)
		: "rcx", "r11", "memory"
	);
	return rax;
}

static inline long syscall4(long rax, long rdi, long rsi, long rdx, long r10) {
	register long reg10 __asm__("r10") = r10;
	__asm__ volatile (
		"syscall"
		: "+a" (rax)
		: "D" (rdi), "S" (rsi), "d" (rdx), "r" (reg10)
		: "rcx", "r11", "memory"
	);
	return rax;
}

static inline long syscall6(long rax, long rdi, long rsi, long rdx, long r10, long r8, long r9) {
	register long reg10 __asm__("r10") = r10;
	register long reg8 __asm__("r8") = r8;
	register long reg9 __asm__("r9") = r9;

	__asm__ volatile (
		"syscall"
		: "+a" (rax)
		: "D" (rdi), "S" (rsi), "d" (rdx), "r" (reg10), "r" (reg8), "r" (reg9)
		: "rcx", "r11", "memory"
	);

	return rax;
}

#endif
