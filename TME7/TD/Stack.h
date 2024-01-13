#pragma once
#include <cstring> // size_t,memset
#include <iostream>
#include <semaphore.h>

namespace pr {
#define STACKSIZE 100

template<typename T>
class Stack {
    T tab [STACKSIZE];
    size_t sz;
    sem_t mutex;
    sem_t sempop;
    sem_t sempush;
public :
    Stack () : sz(0) { 
        sem_init(&mutex,1,1);
        sem_init(&sempop,1,0);
        sem_init(&sempush,1,STACKSIZE);
        memset(tab,0,sizeof (tab));
    }
    
    ~Stack () {
        sem_destroy(&mutex);
        sem_destroy(&sempop);
        sem_destroy(&sempush);
    }

    T pop () {
        sem_wait(&sempop);
        sem_wait(&mutex);
        T toret = tab[--sz];
        sem_post(&mutex);
        sem_post(&sempush);
        return toret;
    }
    
    void push(T elt) {
        sem_wait(&sempush);
        sem_wait(&mutex);
        tab[sz++] = elt;
        sem_post(&mutex);
        sem_post(&sempop);
    }
};
}