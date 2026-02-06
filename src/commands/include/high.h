#include "wrapper.h"
#include "errno.h"
#include "perm.h"

#define O_RDONLY	0
#define O_WRONLY	1
#define O_RDWR		2
#define O_CREAT		0100
#define O_TRUNC		01000


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

static inline int fcopy(const char *src, const char *dst, unsigned short mode) {
	char buf[65536]; // 64KB

	int fsrc = sys_open(src, O_RDONLY, 0);
	if (fsrc < 0) return -1;

	int fdst = sys_open(dst, O_CREAT | O_TRUNC, mode);
	if (fdst < 0) {
		sys_close(fsrc);
		return -1;
	}

	// initial value for bytes read.
	long bytes_read = 0;

	// read chunks per 64KB in loop.
	// bytes variable is how many bytes already read.
	while ((bytes_read = sys_read(fsrc, buf, sizeof(buf))) > 0) {
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
				return -1;
			}

			// let's save the written chunk to bytes_written.
			bytes_written += w;
		}
	}

	return 0;
}

static inline int move(const char *src, const char *dest) {
	struct stat src_stat, dest_stat;

	if (stat(src, &src_stat) < 0) {
		println("Source file not found!");
		return -1;
	}

	if (stat(dest, &dest_stat) < 0) {
		println("Error while getting info of destination path");
		return -1;
	}

	long ret_rname = frename(src, dest);

	if (ret_rname == -EXDEV) {
		fcopy(src, dest, dest_stat.st_mode);
		return 0;
	}

	return 0;
}
