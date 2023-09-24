#include "strutil.h"

int main(){
    const char *s = "Hello World!";
    pr::string s1(s);
    pr::string s2("Hello Wor1d!");

    pr::string str_affecation(s);
    pr::string str_operator = str_affecation;
    pr::string str_copie(str_affecation);

    std::cout << str_affecation << std::endl;
    std::cout << str_operator << std::endl;
    std::cout << str_copie << std::endl;

    std::cout << (s1 == s2) << std::endl;
    std::cout << (s1 < s2) << std::endl;
    return 0;
}