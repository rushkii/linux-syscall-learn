
#include "helper.h"
#include "struct.h"

#define SYS_READ	0
#define SYS_WRITE	1
#define SYS_OPEN	2
#define SYS_CLOSE	3
#define SYS_STAT	4
#define SYS_FSTAT	5
#define SYS_LSTAT	6
#define SYS_EXIT	60
#define SYS_RENAME	82


static inline long sys_read(unsigned int fd, char *buf, unsigned int count) {
	return syscall3(SYS_READ, fd, (long)buf, count);
}

static inline long sys_write(unsigned int fd, const char *buf, unsigned int count) {
	return syscall3(SYS_WRITE, fd, (long)buf, count);
}

static inline long sys_open(const char *filename, int flags, unsigned short mode) {
	return syscall3(SYS_OPEN, (long)filename, flags, mode);
}

static inline long sys_close(unsigned int fd) {
	return syscall1(SYS_CLOSE, fd);
}

static inline long sys_exit(int code) {
	syscall1(SYS_EXIT, code);
    	__builtin_unreachable();
}

static inline long sys_stat(const char *filename, struct stat *statbuf) {
	return syscall2(SYS_STAT, (long)filename, (long)statbuf);
}

static inline long sys_fstat(unsigned int fd, struct stat *statbuf) {
	return syscall2(SYS_STAT, (long)fd, (long)statbuf);
}

static inline long sys_lstat(const char *filename, struct stat *statbuf) {
	return syscall2(SYS_STAT, (long)filename, (long)statbuf);
}

static inline long sys_rename(const char *oldname, const char *newname) {
	return syscall2(SYS_RENAME, (long)oldname, (long)newname);
}
