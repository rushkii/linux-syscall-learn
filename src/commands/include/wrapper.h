
#include "helper.h"
#include "struct.h"
#include "type.h"

#define SYS_READ	0
#define SYS_WRITE	1
#define SYS_OPEN	2
#define SYS_CLOSE	3
#define SYS_STAT	4
#define SYS_FSTAT	5
#define SYS_LSTAT	6
#define SYS_MMAP	9
#define SYS_MUNMAP	11
#define SYS_EXIT	60
#define SYS_RENAME	82
#define SYS_UNLINK	87


static inline long sys_read(uint fd, char *buf, uint count) {
	return syscall3(SYS_READ, fd, (long)buf, count);
}

static inline long sys_write(uint fd, cstr buf, uint count) {
	return syscall3(SYS_WRITE, fd, (long)buf, count);
}

static inline long sys_open(cstr filename, int flags, ushort mode) {
	return syscall3(SYS_OPEN, (long)filename, flags, mode);
}

static inline long sys_close(uint fd) {
	return syscall1(SYS_CLOSE, fd);
}

static inline long sys_exit(int code) {
	syscall1(SYS_EXIT, code);
    	__builtin_unreachable();
}

static inline long sys_stat(cstr filename, struct stat *statbuf) {
	return syscall2(SYS_STAT, (long)filename, (long)statbuf);
}

static inline long sys_fstat(uint fd, struct stat *statbuf) {
	return syscall2(SYS_STAT, (long)fd, (long)statbuf);
}

static inline long sys_lstat(cstr filename, struct stat *statbuf) {
	return syscall2(SYS_STAT, (long)filename, (long)statbuf);
}

static inline void *sys_mmap(void *addr, size_t len, int prot, int flags, int fd, long offset) {
	return (void *)syscall6(SYS_MMAP, (long)addr, len, prot, flags, fd, offset);
}

static inline long sys_munmap(void *addr, size_t len) {
	return syscall2(SYS_MUNMAP, (long)addr, len);
}

static inline long sys_rename(cstr oldname, cstr newname) {
	return syscall2(SYS_RENAME, (long)oldname, (long)newname);
}

static inline long sys_unlink(cstr pathname) {
	return syscall1(SYS_UNLINK, (long)pathname);
}
