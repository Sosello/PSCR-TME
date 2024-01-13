#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main () {
	const int N = 3;
	std::cout << " main pid= " << getpid() << std::endl;

	int n_child = 0;

	for (int i=1, j=N; i<=N && j==N; i++ ) {
		std::cout << " pid= " << getpid() << " ppid= " << getppid() << " i:j " << i << ":" << j << std::endl;
		if(fork() == 0) {
			n_child = 0;
		}else{
			n_child++;
			break;
		}

		for (int k=1; k<=i && j==N ; k++) {
			if ( fork() == 0) {
				n_child = 0;
				j=0;
				std::cout << " pid= " << getpid() << " ppid= " << getppid() << " k:j " << k << ":" << j << std::endl;
			}else{
				n_child++;
			}
		}
	}

	for(int i = 0; i < n_child; i++){
			int status;
			int pid = wait(&status);
			std::cout << " pid= " << getpid() << " ppid= " << getppid() << " child " << i << " with pid " << pid << " terminated " << std::endl;
		}
	std::cout << "Procesus "  << getpid() << " termine " << std::endl;
	return 0;
}