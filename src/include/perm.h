#ifndef PERM_H
#define PERM_H

// set-user-ID (set process effective user ID on execve(2))
#define S_ISUID  04000

/*
* set-group-ID (set process effective group ID on execve(2);
* mandatory locking, as described in fcntl(2); take a new
* file's group from parent directory, as described in
* chown(2) and mkdir(2))
*/
#define S_ISGID  02000

/*
* sticky bit (restricted deletion flag, as described in unlink(2))
*/
#define S_ISVTX  01000

// read by owner
#define S_IRUSR  00400

// write by owner
#define S_IWUSR  00200

/*
* execute/search by owner ("search" applies for directories,
* and means that entries within the directory can be
* accessed)
*/
#define S_IXUSR  00100

// read by group
#define S_IRGRP  00040

// write by group
#define S_IWGRP  00020

// execute/search by group
#define S_IXGRP  00010

// read by others
#define S_IROTH  00004

// write by others
#define S_IWOTH  00002

// execute/search by others
#define S_IXOTH  00001

#endif
