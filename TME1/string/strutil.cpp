#include "strutil.h"

size_t pr::length(const char* src){
    const char* ptor;
    for(ptor=src; *ptor; ++ptor){}
    return ptor - src;
}

char* pr::newcopy(const char* src){
    char *dst = new char[pr::length(src)];
    memcpy(dst, src, pr::length(src)+1);
    return dst;
}

pr::string::string(const char* s){
    str = pr::newcopy(s);
}

pr::string::string(const pr::string& ori){
    str = pr::newcopy(ori.str);
}

pr::string::~string(){
    delete[] str;
}

pr::string& pr::string::operator=(const pr::string& ori){
    if(this != &ori){
        delete[] str;
        str = pr::newcopy(ori.str);
    }
    return *(this);
}

size_t pr::string::length() const{
    return pr::length(str);
}

std::ostream& pr::operator<<(std::ostream&os, const pr::string &s){
    return os << s.str;
}

int pr::compare(const char* a, const char* b){
    while(*a != '\0'){
        if(*a < *b)
            return -1;
        if(*a >*b)
            return 1;
        a++;
        b++;
    }
    return 0;
}

bool pr::operator==(const string &a,const string &b){
    return !pr::compare(a.str,b.str);
}

bool pr::string::operator<(const string & b) const{
    return pr::compare(str, b.str)<0;
}

