#include "List.h"
#include <string>
#include <iostream>
#include <cstring>

int main () {

	std::string abc = "abc";
	//FAUTE : Pas de '\0' à la fin de chaine de charactère
	char * str = new char [4];
	str[0] = 'a';
	str[1] = 'b';
	str[2] = 'c';
	str[3] = '\0';
	//FAUTE : size_t non negatif
	int i = 0;

	if (! strcmp (str, abc.c_str())) {
		std::cout << "Equal !" << std::endl;
	}

	pr::List list;
	list.push_front(abc);
	list.push_front(abc);

	std::cout << "Liste : " << list << std::endl;
	std::cout << "Taille : " << list.size() << std::endl;

	// Affiche à l'envers
	for (i= list.size() - 1 ; i >= 0 ; i--) {
		std::cout << "elt " << i << ": " << list[i] << std::endl;
	}

	// liberer les char de la chaine
	//FAUTE : Pas besoin de liberer chaque caractère 
	/*
	for (char *cp = str ; *cp ; cp++) {
		delete cp;
	}
	*/
	// et la chaine elle meme
	// FAUTE : Ajouter une [] pour liberer tableau
	delete []str;

}
