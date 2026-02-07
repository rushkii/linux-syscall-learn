#include "wrapper.h"
#include "errno.h"
#include "perm.h"
#include "type.h"

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

static inline void println(cstr buf) {
	size_t len = 0;

	while (buf[len]) len++;

	sys_write(1, buf, len);
	sys_write(1, "\n", 1);
}

static inline long stat(cstr path, struct stat *src_stat) {
	return sys_stat(path, src_stat);
}

static inline int frename(cstr oldname, cstr newname) {
	return sys_rename(oldname, newname);
}

static inline int unlink(cstr pathname) {
	return sys_unlink(pathname);
}

static inline void *mmap(void *addr, size_t len, int prot, int flags, int fd, long offset) {
	return (void *)sys_mmap(addr, len, prot, flags, fd, offset);
}

static inline long munmap(void *addr, size_t len) {
	return sys_munmap(addr, len);
}

static inline int fcopy(cstr src, cstr dst) {
	// create a virtual address with the given BUF_SIZE
	char *buf = (char *)mmap(0, BUF_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if ((long)buf < 0) return -1;

	// open the source path (argv[1])
	int fsrc = sys_open(src, O_RDONLY, 0);
	if (fsrc < 0) {
		sys_munmap(buf, BUF_SIZE);
		return -1;
	}

	struct stat src_stat;
	ushort mode;

	if (stat(src, &src_stat) < 0) {
		mode = 0644;
	} else {
		mode = src_stat.st_mode;
	}

	// open the destination path (argv[2])
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

	// clean up
	sys_close(fsrc);
	sys_close(fdst);
	sys_munmap(buf, BUF_SIZE);

	return (bytes_read < 0) ? -1 : 0;
}

static inline int move(cstr src, cstr dest) {
	// try to move with rename first
	long ret = frename(src, dest);

	// if it's sucess then no need to continue
	if (ret == 0) return 0;

	// this part if using cross-device method
	// e.g moving from disk A to disk B
	if (ret == -EXDEV) {

		// this method will use read source file and write dest file
		// definitely copy file from source to destination
		if (fcopy(src, dest) < 0) return -1;

		// and then remove the source file.
		if (unlink(src) < 0) return -1;

		return 0;
	}

	return -1;
}
