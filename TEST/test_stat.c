#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

struct stat file_stat;

int main (int argc, char ** argv) {

    int tubeDesc[2];
    if (pipe (tubeDesc) == -1) {
        perror ("pipe"); exit (1);
    }
    if (fstat (tubeDesc[0],&file_stat) == -1) {
        perror ("fstat"); exit (2);
    }
    if (S_ISFIFO (file_stat.st_mode)) {
        printf ("il s'agit d'un tube \n");
        printf ("num. inode %d \n", (int)file_stat.st_ino);
        printf ("nbr. de liens %d \n", (int)file_stat.st_nlink);
        printf ("Taille : %d \n", (int) file_stat.st_size);
    }
    return EXIT_SUCCESS;
}