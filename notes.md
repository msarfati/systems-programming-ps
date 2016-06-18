# Notes

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
