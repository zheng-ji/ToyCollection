#ifndef _SHORT_H_
#define _SHORT_H_

#include <map>
#include <string.h>
#include <string>
using namespace std;


class Shortener {
    private:
        int m_next_id;
        string m_root;
        map<string,string>m_urls;
        map<string,string>m_tags;

    public:
        Shortener():m_root("https://t.cn/"),m_next_id(1000){}
        ~Shortener(){}

        string shorten(string url);
        string baseN(int num);
        string lstrip(string other);

};
#endif
