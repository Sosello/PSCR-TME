#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <vector>
#include <forward_list>
#include <unordered_map>
#include "HashTable.h"

using namespace pr;
int mainp1 () {
	using namespace std;
	using namespace std::chrono;

	ifstream input = ifstream("WarAndPeace.txt");

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;

	size_t nombre_lu = 0;
	// prochain mot lu
	string word;
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re( R"([^a-zA-Z])");

	//vector<string> vec;
	vector<pair<string,int>> bucket;

	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);

		// word est maintenant "tout propre"
		if (nombre_lu % 100 == 0)
			// on affiche un mot "propre" sur 100
			cout << nombre_lu << ": "<< word << endl;
		nombre_lu++;
		
		
		bool wordexist = false;
		
		for(auto it = bucket.begin(); it!= bucket.end(); ++it){
			if(it->first==word){
				wordexist = true;
				it->second += 1;
				break;
			}
		}
		if(!wordexist)
			bucket.push_back(make_pair(word,1));
		
	}
	input.close();

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << nombre_lu << " words." << endl;

	bool warexist = false;
	bool peacexist = false;
	bool totoexist = false;
	for(auto it:bucket){
		if(it.first=="war"){
			cout << "We found " << it.second << " war in text." <<endl;
			warexist = true;
			break;
		}
	}
	if(!warexist)
		cout << "We can't find war in text." <<endl;
	
	for(auto it:bucket){
		if(it.first=="peace"){
			cout << "We found " << it.second << " peace in text." <<endl;
			peacexist = true;
			break;
		}
	}
	if(!peacexist)
		cout << "We can't find peace in text." <<endl;

	for(auto it:bucket){
		if(it.first=="toto"){
			cout << "We found " << it.second << " toto in text." <<endl;
			totoexist = true;
			break;
		}
	}
	if(!totoexist)
		cout << "We can't find toto in text." <<endl;
	
    return 0;
}


int mainp2 () {
	using namespace std;
	using namespace std::chrono;

	ifstream input = ifstream("WarAndPeace.txt");

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;

	size_t nombre_lu = 0;
	// prochain mot lu
	string word;
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re( R"([^a-zA-Z])");
	
	HashTable<string,int> map = HashTable<string,int>(5000);
	
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);

		// word est maintenant "tout propre"
		if (nombre_lu % 100 == 0)
			// on affiche un mot "propre" sur 100
			cout << nombre_lu << ": "<< word << endl;
		nombre_lu++;
		
		auto it = map.get(word);
		if(it==nullptr){
			map.put(word,1);
		}else{
			(*it)++;
		}
		
		map.grow();
		
	}
	input.close();

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << nombre_lu << " words." << endl;

	if(map.get(string("war"))){
		cout << "We found " << *map.get(string("war")) << " war in text." <<endl;
	}else{
		cout << "We can't find war in text." <<endl;
	}

	vector<pair<string,int>> vec;
	vec.reserve(5000);
/*
	auto itr = map.begin();
	vec.push_back(make_pair((*itr).key_,(*itr).value_));
	*/


	for(auto itr=map.begin();itr!=map.end();++itr){
		vec.push_back(make_pair((*itr).key_,(*itr).value_));
	}

	sort(vec.begin(),vec.end(),[](const pair<string,int>& a, const pair<string,int>& b){return a.second>b.second;});

	for(int i=0;i<10;i++){
		cout << vec[i].first << " " << vec[i].second << endl;
	}
	

    return 0;
}

int main () {
	using namespace std;
	using namespace std::chrono;

	ifstream input = ifstream("WarAndPeace.txt");

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;

	size_t nombre_lu = 0;
	// prochain mot lu
	string word;
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re( R"([^a-zA-Z])");

	unordered_map<string,int> map;
	unordered_map<int,forward_list<string>> rlmap;

	
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);

		auto it = map.find(word);
		if(it!=map.end()){
			it->second++;
		}else{
			map.emplace(word,1);
		}

		// word est maintenant "tout propre"
		if (nombre_lu % 100 == 0)
			// on affiche un mot "propre" sur 100
			cout << nombre_lu << ": "<< word << endl;
		nombre_lu++;
		
	}
	input.close();

	for (const auto& pair : map) {
        const std::string& word = pair.first;
        int frequency = pair.second;

        // Ajoutez le mot à la liste associée à la fréquence dans la table inversée
        rlmap[frequency].push_front(word);
    }

    // Vous pouvez maintenant accéder à la liste de mots pour une fréquence donnée
    int frequencyToLookup = 298;
    if (rlmap.find(frequencyToLookup) != rlmap.end()) {
        std::cout << "Mots avec une fréquence de " << frequencyToLookup << ": ";
        for (const std::string& word : rlmap[frequencyToLookup]) {
            std::cout << word << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "Aucun mot avec une fréquence de " << frequencyToLookup << " trouvé." << std::endl;
    }

    return 0;

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << nombre_lu << " words." << endl;



    return 0;
}