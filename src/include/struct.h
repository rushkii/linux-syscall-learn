#ifndef STRUCT_H
#define STRUCT_H

struct stat {
    unsigned long st_dev;       // Device ID (filesystem identifier)
    unsigned long st_ino;       // Inode number
    unsigned long st_nlink;     // Number of hard links
    unsigned int  st_mode;      // File type and permissions
    unsigned int  st_uid;       // Owner UID
    unsigned int  st_gid;       // Owner GID
    unsigned int  __pad0;       // Padding
    unsigned long st_rdev;      // Device ID (if special file)
    long          st_size;      // File size in bytes
    long          st_blksize;   // Block size for I/O
    long          st_blocks;    // Number of 512-byte blocks
    unsigned long st_atime;     // Last access time
    unsigned long st_atime_nsec;
    unsigned long st_mtime;     // Last modification time
    unsigned long st_mtime_nsec;
    unsigned long st_ctime;     // Last status change time
    unsigned long st_ctime_nsec;
    long          __unused[3];
};

#endif
