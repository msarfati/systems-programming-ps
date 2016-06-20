#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

void main()
{
    struct stat sb;     // Stat buffer

    stat("foo", &sb);

    printf("last accessed: %s", ctime(&sb.st_atime));
    printf("last modified: %s", ctime(&sb.st_mtime));
    printf("last changed: %s", ctime(&sb.st_ctime));
}
