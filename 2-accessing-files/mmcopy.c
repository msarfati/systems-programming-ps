/*
Copying using mmap

1) input file "foo" is mmap'd (or copied) into in-memory into *src's buffer
2) output file "bar" is mmap'd into memory into *dst's buffer
2) memcpy() copies the mmap'd bytes from *src's buffer to *dst's buffer
3) msync() pushes *dst buffer out to the file
*/

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char *src, *dst;
    int fin, fout;
    size_t size;

    fin = open("foo", O_RDONLY);
    if (fin < 0) {
        perror("foo");
        exit(1);
    }

    size = lseek(fin, 0, SEEK_END);
    // Get size of file -- this will be used to specify the size of the two mappings

    src = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fin, 0);
    if (src == MAP_FAILED) {
        perror("mmap");
        exit(2);
    }

    fout = open("bar", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fout < 0) {
        perror("bar");
        exit(1);
    }

    if (ftruncate(fout, size) == -1) {  // makes size of output file = size of input file
        perror("ftruncate");
        exit(3);
    }

    dst = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fout, 0);
    if (dst == MAP_FAILED) {
        perror("mmap");
        exit(4);
    }

    memcpy(dst, src, size);     // copy bytes between mappings
    if (msync(dst, size, MS_SYNC) == -1) {
        perror("msync");
        exit(5);
    }

    exit(0);
}