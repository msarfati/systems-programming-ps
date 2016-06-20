# Chapter 3: Managing Files and Directories

## File System Structure
- after an empty file system is created, an **inode table** is creatd
- an **inode** holds a file's attributes: permissions, owners & groups, timestamps
- inode also holds pointer information (blocks) to the actual data for the file
- a directory is a **file** (with its own **inode**), and a directory contains links to the file.
- Directories associate a name with an inode number in the inode table
- inode table data structures in modern *Nix distros are very dynamic

## Examining File Attributes
- Two main functions for accessing a file's attributes: `stat` and `fstat`
- `man 3 stat`, `man 3 fstat`
```c
int stat(
    const char *restrict path,
    struct stat *restrict buf       // Stat struct
);

int fstat(          // returns 0 on success, -1 on failure
    int fildes,     // Open file descriptor
    struct stat *buf
);
```

### `stat` Struct
- from `man 2 stat`:
```c
 struct stat {
     dev_t     st_dev;         /* ID of device containing file */
     ino_t     st_ino;         /* inode number */
     mode_t    st_mode;        /* file type and mode (access permissions r-w-x) */
     nlink_t   st_nlink;       /* number of hard links */
     uid_t     st_uid;         /* user ID of owner */
     gid_t     st_gid;         /* group ID of owner */
     dev_t     st_rdev;        /* device ID (if special file) */
     off_t     st_size;        /* total size, in bytes */
     blksize_t st_blksize;     /* blocksize for filesystem I/O */
     blkcnt_t  st_blocks;      /* number of 512B blocks allocated */

     /* Since Linux 2.6, the kernel supports nanosecond
        precision for the following timestamp fields.
        For the details before Linux 2.6, see NOTES. */

     struct timespec st_atim;  /* time of last access (read) */
     struct timespec st_mtim;  /* time of last modification (appending/rewriting file) */
     struct timespec st_ctim;  /* time of last status change (chmod) */
 #define st_atime st_atim.tv_sec      /* Backward compatibility */
 #define st_mtime st_mtim.tv_sec
 #define st_ctime st_ctim.tv_sec
 };
```
- **Creation time of file is not recorded!**
- copying a file also updates access time
- inode is written to when a file is read, in order to update `st_atim` (access time). This creates problem on flash memory that have limited read/write cycles.
- thus, you can specify the `noatime` option to `mount`

## File Type and Permissions
### st_mode 
- `st_mode` contains the following bits:

0010000 | 0020000 | 0040000 | 0060000 | 0100000 | 0120000 | 0140000 | s | g | t | r, w , x | r , w , x | r , w , x |
--- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
FIFO (pipe) | character device | directory | block device | regular file | symbolic link | socket | set UID | set GID | Sticky bit | Owner permissions | Group permissions | Other permissions
| | | | | | | | S_ISUID | S_ISGID | S_ISVTX | S_IRUSR, S_IWUSR, S_IXUSR | S_IWGRP, S_IRGRP S_IXGRP | S_IROTH, S_IWOTH, S_IXOTH
- Sticky bit `t` often ignored on ordinary files... was important on UNIX to protect a file from being deleted
- `s` and `g` are the heart of privilege escalation. Tells executable to run with permissions of group (`s`) or owner (`g`)
- 0010000 through to 0140000 (top 4 bits) specify file type