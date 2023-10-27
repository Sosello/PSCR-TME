#include "Banque.h"

#include <iostream>

using namespace std;

namespace pr {
/**
 * Transfert val du compte deb vers le compte cred
 * Cette version peut provoquer des interblocages (deadlocks)
 * Mais quand le nombre de threads est grand qui déborde l'intervalle de int, le programme ne termine pas
 * Par exemple, avec 100000 threads, le programme ne termine pas
 */

void Banque::transfert(size_t deb, size_t cred, unsigned int val) {
	unique_lock<mutex> g(m);
	Compte & debiteur = comptes[deb];
	Compte & crediteur = comptes[cred];
	unique_lock<recursive_mutex> g1(debiteur.getMutex(), defer_lock);
	unique_lock<recursive_mutex> g2(crediteur.getMutex(), defer_lock);
	lock(g1,g2);
	if (debiteur.debiter(val)) {
		crediteur.crediter(val);
	}
}
size_t Banque::size() const {
	unique_lock<mutex> g(m);
	return comptes.size();
}
bool Banque::comptabiliser (int attendu) const {
	unique_lock<mutex> g(m);
	int bilan = 0;
	int id = 0;
	for (const auto & compte : comptes) {
		if (compte.getSolde() < 0) {
			cout << "Compte " << id << " en négatif : " << compte.getSolde() << endl;
		}
		bilan += compte.getSolde();
		id++;
	}
	if (bilan != attendu) {
		cout << "Bilan comptable faux : attendu " << attendu << " obtenu : " << bilan << endl;
	}
	return bilan == attendu;
}

}


