#include <cstring>
#include <iostream>
#pragma once

namespace pr{
    size_t length(const char* s);
    char* newcopy(const char* s);

    class string{
    private:
        char *str;
    public:
        string(const char* str);
        string(const string& ori);
        string& operator=(const string& ori);
        ~string();
        size_t length() const;
        friend  std::ostream &operator<<(std::ostream&os, const string &s);
        friend  int compare(const char* a, const char* b);
        friend bool operator==(const string &a,const string &b);
        bool operator<(const string & b) const;
    };
    
    std::ostream &operator<<(std::ostream&os, const string &s);
    int compare(const char* a, const char* b);
    bool operator==(const string &a,const string &b);
}


