# Chapter 2: Accesssing Files

## Man pages
- Section 3 of `man` will open info about **library calls**

```bash
man 3 open
```
- section 2 is about **system calls**
- More information found:

```bash
man man
```

## Low-level IO

### open()
- Anatomy of the `open` function:
```c
fd = open(
	name,	// path name of file to be opened
	flags,	// must be: O_RDONLY, O_WRONLY, RDWR. May also include; O_APPEND, O_CREAT, O_TRUC
	mode	// Access mode, ie, permissions (chmod). Limited to the process's *umask*
);
```
- at the end of this call, fd is assigned to file descriptor "3", or the lowest available *file descriptor*.
- By default, stdin is given file descriptor 0
- while running, a program is not usually aware of redirecting streams

- more info at `man 3 open`

#### A note on system file oflags (symbolic constants)
- When flags are used, they are typically designed to be bitwise or'd. The following table represents this:
	O_CREAT		0100
	O_TRUNC		01000
	O_APPEND	02000

```c
fd = open("foo", O_RDWR | O_TRUNC | O_APPEND);
```

### write() - unbuffered output
```c
write(
	fd,		// file descriptor
	buffer,	// pointer to buffer where data is held
	count	// size of the buffer. DON'T specify length longer than buffer, leads to buffer overflow. Compiler won't complain
);
```
- No format conversion or user-space buffering

### read() - unbuffered input
```c
read(
	fd,
	buffer,
	count
);
```
- Raw binary, count should not be bigger than buffer
- read returns 0 on EOF (end-of-file)

### close()
```c
close(fd)
```
- closes and makes availiable for reuse
- descriptors implicitly closed when process terminates
- closing is good practice, and essential in long-running process
- **there's a limit on how many descriptors a process can have open**

## Random Access
### lseek()
- All of the above commands deal with sequential access
- File pointers will always begin at the beginning, unless the flag is set to `O_APPEND`
- more info: man 3 lseek
```c
lseek(
	fd,
	offset,	// byte offset, positive or negative
	whence	// offset relative to: SEEK_SET (start of file), SEEK_CUR (current), SEEK_END (EOF)
);

// examples:
lseek(fd, 100, SEEK_CUR);  // Start at the current position in file, and writes 100 bytes
lseek(fd, 100, SEEK_SET);  // Start at beginning of file, and writes 100 bytes
lseek(fd, -100, SEEK_END);  // Start at end of file, writes 100 bytes backwards

lseek(fd, 100, SEEK_END);  // File is conceptually extended but nothing changed on disk, until it's written to will 
```

## Buffered and formatted IO
- C lang doesn't define any features to handle IO, library evolved: "standard io library"

```c
fd = fopen( // fopen
    name,   // Pathname
    mode    // string: "r", "W", "r+", "b" (important for Windows)
);
// fd returns a pointer to a file: * FILE
// returns a null pointer on error

/* Output */
fwrite(		// Returns number of elements actually written
	buffer,
	size,
	num,	// number of objects
	fd);	// FILE * descriptor returned by fopen()

/* Input */
fread(		// returns number of elements actually read
	buffer,
	size,
	num,	// number of objects
	fd);	// FILE *

/* Closing */
fclose(fd);	// closes descriptor, flushes buffered data.. But again, limit on how many descriptors a ps can have open, so best practice to close file-descriptor when done
```


### Difference between low-level IO and standard library

Feature	| Low-Level IO | Standard Lib
--- | --- | ---
Read/write access | `open(), close(), read(), write()`	| `fopen(), fclose(), fread(), fwrite()`
Random Access | `lseek()` | `fseek()`
Type of descriptor | `int` | FILE *
User-space buffering? | No | Yes
Part of C Standard? | No | Yes

- **Does not dive into the kernel every time it's called. Instead, output is accumulated into a buffer, and then finally makes the kernel call to write**
- by timing `rawio.c`'s output to a file by using low-level calls, we demonstrate that most time is spent in kernel space
- The same routine with using standard library (ie, buffered IO) is **much** faster and occurs entirely in
- There's a heavy overhead involved in making a system call. Thus, you want to buffer up your calls to the kernel if you can.

### printf() and friends
- Generates a formatted string and writes to `stdout` (fd==1)

```c
// printf example
char *name = "Sharon";
int age = 45;
double wage = 24500.00;
printf("%12s is a %d and earns %f\n", name, age, wage);
```


**printf() format codes** |  |
--- | --- |
`%d` | decimal integer
`%8d` | right-justified in 8 character field
`%-8d` | left-justified in 8 character field
`%s` | string
`%12.3f` | double in 12 characater field width w/ 3 digits after decimal 

- `man 3 printf` for more info

#### fprintf() and sprintf()
```c
fd = fopen(...);
fprintf( //
	fd,	// Use stderr to output error message
	"hello world");

char[100] buf;
sprintf(	// Formats string into memory, used often with GUI apps to output to text box or building an SQL query
	buf,
	"Hello");
// Again, be careful about not overflowing the bfufer
// Can also use snprintf() to specify number of bytes
```

### Scatter / Gather IO
- implemented in *readv()* and *writev()*
- Gathers and writes to file in one atomic operation

```c
ssize_t writev(		/* Data written atomically to the file, buffers
					guarenteed to end up sequential, even if
					another process is concurrently appending data to file */
	int fildes,		// file descriptor
	const struct iovec *iov, /* Points to array of IOVec structures (iov_base + iov_len) */
	int iocount // length of the array
);
```

### Mapping Files into Memory: mmap()

```c
void mmap(
	void *addr,	// set to NULL to allow kernel to choose address
	size_t len,	// length of mapping
	int prot,	// Some combination of PROT_READ and PROT_WRITE
	int flags,	// MAP_SHARED, MAP_PRIVATE (if other processes can see this)
	int fildes,		// File descriptor from open()
	off_t offset	// multiple of page size (often 0)
);

/* mmap returns the address where the file has been mapped
a lot like malloc() */
```

- see [mmcopy.c](./2-accessing-files/mmcopy.c) for an example.