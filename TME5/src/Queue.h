#ifndef SRC_QUEUE_H_
#define SRC_QUEUE_H_

#include <cstdlib>
#include <mutex>
#include <condition_variable>
#include <cstring>

namespace pr {

// MT safe version of the Queue, non blocking.
template <typename T>
class Queue {
	T ** tab;
	const size_t allocsize;
	std::condition_variable cv;
	size_t begin;
	size_t sz;
	mutable std::mutex m;
	mutable std::mutex m2;
	bool isblocking = false;

	// fonctions private, sans protection mutex
	bool empty() const {
		std::unique_lock<std::mutex> lg(m2);	
		return sz == 0;
	}
	bool full() const {
		std::unique_lock<std::mutex> lg(m2);
		return sz == allocsize;
	}
public:
	Queue(size_t size) :allocsize(size), begin(0), sz(0) {
		tab = new T*[size];
		memset(tab, 0, size * sizeof(T*));
	}
	size_t size() const {
		std::unique_lock<std::mutex> lg(m);
		return sz;
	}

	void Setblocking(bool b) {
		std::unique_lock<std::mutex> lg(m);
		isblocking = b;
		cv.notify_all();
	}
	T* pop() {
		std::unique_lock<std::mutex> lg(m);
		while (empty()&&isblocking) {
			cv.wait(lg);
		}
		if(empty())
			return nullptr;

		if(full())
			cv.notify_all();
		auto ret = tab[begin];
		tab[begin] = nullptr;
		sz--;
		begin = (begin + 1) % allocsize;
		return ret;
	}
	bool push(T* elt) {
		std::unique_lock<std::mutex> lg(m);
		while (full()&&isblocking) {
			cv.wait(lg);
		}
		if(full())
			return false;

		if(empty())
			cv.notify_all();
		tab[(begin + sz) % allocsize] = elt;
		sz++;
		return true;
	}
	~Queue() {
		// ?? lock a priori inutile, ne pas detruire si on travaille encore avec
		for (size_t i = 0; i < sz; i++) {
			auto ind = (begin + i) % allocsize;
			delete tab[ind];
		}
		delete[] tab;
	}
};

}

#endif /* SRC_QUEUE_H_ */
