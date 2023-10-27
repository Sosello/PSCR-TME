#include "Banque.h"
#include <iostream>

using namespace std;
using namespace pr;

const int NB_THREAD = 3000;
const int SOLDEINITIAL = 1000;
const int NB_COMPTE = 100;

void transaction(Banque & b) {
	int i = rand()%b.size();
	int j = rand()%b.size();
	int m = rand()%100;
	b.transfert(i,j,m);
	std::this_thread::sleep_for(std::chrono::milliseconds(rand()%20));
}

int main () {
	vector<thread> threads;
	threads.reserve(NB_THREAD);
	Banque b(NB_COMPTE, SOLDEINITIAL);
	
	for(int i = 0; i < NB_THREAD; i++) {
		threads.emplace_back(transaction, ref(b));
		std::cout << "Thread " << i << " créé" << std::endl;
		b.comptabiliser(NB_COMPTE*SOLDEINITIAL);
	}
	



	for(int i = 0; i < NB_THREAD; i++) {
		threads[i].join();
		std::cout << "Thread " << i << " terminé" << std::endl;
	}

	return 0;
}

