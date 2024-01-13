#include <iostream>
#include <unistd.h>

int main () {
    int N = 4;
    int i = 0;
    std::cout << "pid=" << getpid() << std::endl;
    while (i < N) {
        i++;
        int j = i;
        if (fork() == 0) {
            if (i%2==0) {
                N = i - 1;
                i = 0;
            } else {
                N = 0;
            }
            std::cout << "pid=" << getpid() << " ppid=" << getppid() << " j=" << j <<
            " N=" << N << std::endl;
        }  
    }
    return 0;
}