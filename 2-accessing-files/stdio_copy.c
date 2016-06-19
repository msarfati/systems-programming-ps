#include <stdio.h>
#define BSIZE 16384

void main() {
    FILE *fin, *fout;   // I/O handles
    char buf[BSIZE];
    int count;

    fin = fopen("foo", "r");
    fout = fopen("bar", "w");

    while ((count = fread(buf, 1, BSIZE, fin)) > 0) {
        fwrite(buf, 1, count, fout);
    }

    close(fin);
    close(fout);
}
