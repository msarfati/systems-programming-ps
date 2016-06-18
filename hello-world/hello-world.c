#include <fcntl.h>

int main(int argc, char const *argv[])
{
    int fd;

    fd = open("foo", O_WRONLY | O_CREAT, 0644);
    write(fd, "Hello world!", 12);
    close(fd);

    return 0;
}