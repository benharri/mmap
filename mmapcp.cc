#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
        int s, d;
        struct stat st;
        void *sp, *dp;
        s = open(argv[1], O_RDONLY);
        if (s == -1) {
                perror("open source");
                exit(1);
        }
        d = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644);
        if (d == -1) {
                perror("open destintation");
                exit(1);
        }
        if (fstat(s, &st)) {
                perror("stat source");
                exit(1);
        }
        if (ftruncate(d, st.st_size)) {
                perror("truncate destination");
                exit(1);
        }
        sp = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, s, 0);
        if (sp == MAP_FAILED) {
                perror("map source");
                exit(1);
        }
        dp = mmap(NULL, st.st_size, PROT_WRITE | PROT_READ, MAP_SHARED, d, 0);
        if (dp == MAP_FAILED) {
                perror("map destintation");
                exit(1);
        }
        memcpy(dp, sp, st.st_size);
        return 0;
}