#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>

char * path = nullptr;

void handleCtrlC (int sig) {
    unlink(path);
    exit(0);
}


int write (const std::string & s, int fd) {
    size_t sz = s.length();
    if (write (fd, &sz, sizeof(sz)) != sizeof(sz)) {
        perror("write sz");
        return -1;
    }
    if (write (fd, s.data(), sz) != sz) {
        perror("write data");
        return -1;
    }
    std::cout << "written string len=" << sz << " :" << s << std::endl;
    return sz;
}



int main (int argc, char ** argv) {
    if (argc < 2) {
        std::cerr << "Provide a name for the pipe." << std::endl;
    }
    if ( mkfifo(argv[1],S_IRUSR|S_IWUSR) != 0) {
        perror("mkfifo problem");
        return 1;
    }
    path = argv[1];
    int fd = open(argv[1],O_WRONLY);
    if (fd < 0) {
        perror("open problem");
        return 1;
    }
    signal(SIGINT, handleCtrlC);
    while (true) {
        std::string s;
        std::cin >> s;
        if (write(s,fd) <0) {
            break;
        }
    }
    return 0;
}