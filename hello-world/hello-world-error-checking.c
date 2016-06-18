#include <fcntl.h>

void main()
{
    int fd;

    fd = open("foo", O_WRONLY | O_CREAT, 0644);
    if (fd < 0) {
        printf("error: %d\n", errno);
        perror("foo");
        exit(1);
    }
    write(fd, "Hello world!", 12);
    close(fd);
}