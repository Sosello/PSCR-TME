#ifndef BARRIER_H_
#define BARRIER_H_

#include <mutex>
#include <condition_variable>

namespace pr {

// Une barriere pour attendre que tous les threads aient fini leur travail.
class Barrier {
    std::mutex m;
    std::condition_variable cv;
    int cpt;
    const int goal;
public:
    Barrier(int goal) : cpt(0), goal(goal) {}
    void done(){
        std::unique_lock<std::mutex> lg(m);
        cpt++;
        if (cpt == goal){
            cv.notify_all();
        }
        //std::cout << cpt << std::endl;
    }
    void waitFor(){
        std::unique_lock<std::mutex> lg(m);
        while (cpt != goal){
            cv.wait(lg);
        }
    }
};
}

#endif /* BARRIER_H_ */