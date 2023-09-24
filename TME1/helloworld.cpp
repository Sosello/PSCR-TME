#include <iostream>

int main(){

    int* tab = new int[10];
    for(int i=0; i<10; i++){
        tab[i] = i;
        std::cout << tab[i] << " ";
    }

    std::cout << std::endl;
    for(int i=9; i>=0; i--){
        if(tab[i] - tab[i-1]!=1){
            std::cout << tab[i];
            std::cout << "probleme !";
        }
    }

    delete[] tab;

    return 0;
}