#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>


int read (std::string & s, int fd) {
    size_t sz = 0;
    int rd = read (fd, &sz, sizeof(sz));
    if (rd ==0) {
        // Ceci arrivera ssi il n’y a plus d’ecrivain => introduit la notion de EOF
        return 0;
    } else if ( rd != sizeof(sz)) {
        perror("read error");
        return -1;
    }
    char buff [sz];
    if (read (fd, buff, sz) != sz) {
        perror("read error");
        return -1;
    }
    /* ctor a deux arguments, pour éviter de couper la chaine s’il y a des \0 dedans
    */
    s = std::string(buff, sz);
    return sz;
}

int main (int argc, char ** argv) {
    if (argc < 2) {
        std::cerr << "Provide a name for the pipe." << std::endl;
    }
    int fd = open(argv[1],O_RDONLY);
    if (fd < 0) {
        perror("open problem");
        return 1;
    }
    while (true) {
        std::string s;
        int rd = read(s,fd);
        if (rd == 0) {
            break;
        }
        std::cout << s << std::endl;
    }
    return 0;
}