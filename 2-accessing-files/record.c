// Random Access example
#include <unistd.h>
#include <fcntl.h>

struct record {
    int id;
    char name[80];
};

void main()
{
    int fd, size = sizeof(struct record);  // size is set to the number of bytes in the record
    struct record info;

    fd = open(
        "datafile",
        O_RDWR  // Reading and writing
    );

    lseek(fd, size, SEEK_SET);  // Skip byte equivalent of one record
    read(fd, &info, size);      // Read second record

    info.id = 99;
    lseek(fd, -size, SEEK_CUR); // Seek backward (backspace)
    write(fd, &info, size); // write modified record

    close(fd);
}