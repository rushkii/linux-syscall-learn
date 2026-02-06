#include "wrapper.h"
#include "errno.h"
#include "perm.h"

#define O_RDONLY	0
#define O_WRONLY	1
#define O_RDWR		2
#define O_CREAT		0100
#define O_TRUNC		01000

#define PROT_READ	0x1
#define PROT_WRITE	0x2
#define MAP_PRIVATE	0x02
#define MAP_ANONYMOUS	0x20

#define BUF_SIZE 65536

static inline void println(const char *buf) {
	unsigned long len = 0;

	while (buf[len]) len++;

	sys_write(1, buf, len);
	sys_write(1, "\n", 1);
}

static inline long stat(const char *path, struct stat *src_stat) {
	return sys_stat(path, src_stat);
}

static inline int frename(const char *oldname, const char *newname) {
	return sys_rename(oldname, newname);
}

static inline int unlink(const char *pathname) {
	return sys_unlink(pathname);
}

static inline void *mmap(void *addr, unsigned long len, int prot, int flags, int fd, long offset) {
	return (void *)sys_mmap(addr, len, prot, flags, fd, offset);
}

static inline long munmap(void *addr, unsigned long len) {
	return sys_munmap(addr, len);
}

static inline int fcopy(const char *src, const char *dst, unsigned short mode) {
	char *buf = (char *)mmap(0, BUF_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if ((long)buf < 0) return -1;

	int fsrc = sys_open(src, O_RDONLY, 0);
	if (fsrc < 0) {
		sys_munmap(buf, BUF_SIZE);
		return -1;
	}

	int fdst = sys_open(dst, O_WRONLY | O_CREAT | O_TRUNC, mode);
	if (fdst < 0) {
		sys_close(fsrc);
		sys_munmap(buf, BUF_SIZE);
		return -1;
	}

	// initial value for bytes read.
	long bytes_read = 0;

	// read chunks per 64KB in loop.
	// bytes variable is how many bytes already read.
	while ((bytes_read = sys_read(fsrc, buf, BUF_SIZE)) > 0) {
		// initial value for bytes written.
		long bytes_written = 0;

		// bytes variable has the total size of the source file
		// so, we will write to destination file until all written,
		// it's literally a copy content.
		while (bytes_written < bytes_read ) {
			// w variable is how many bytes are written.
			long w = sys_write(fdst, buf + bytes_written, bytes_read - bytes_written);

			// if error, close all file (src and dst).
			if (w < 0) {
				sys_close(fsrc);
				sys_close(fdst);
				sys_munmap(buf, BUF_SIZE);
				return -1;
			}

			// let's save the written chunk to bytes_written.
			bytes_written += w;
		}
	}

	sys_close(fsrc);
	sys_close(fdst);
	sys_munmap(buf, BUF_SIZE);

	return (bytes_read < 0) ? -1 : 0;
}

static inline int move(const char *src, const char *dest) {
	struct stat src_stat;

	if (stat(src, &src_stat) < 0) {
		println("Source file not found!");
		return -1;
	}

	long ret = frename(src, dest);

	if (ret == 0) return 0;

	if (ret == -EXDEV) {
		if (fcopy(src, dest, src_stat.st_mode) < 0) {
			println("Failed while trying to copy the file");
			return -1;
		}

		if (unlink(src) < 0) {
			println("Failed while trying to delete the source file");
			return -1;
		}

		return 0;
	}

	println("Failed to move file");
	return -1;
}
