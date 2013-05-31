#include "short.h"

const string g_dict = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

string Shortener::baseN(int num) {
    if (num == 0)
        return "0";
    else
        return lstrip(baseN(num / 62)) + g_dict[num % 62];
}

string Shortener::lstrip(string other) {
    const char * tmp = other.c_str();
    while(*tmp == '0') {
        tmp ++ ;
    }
    return tmp;
}

string Shortener::shorten(string url) {
    map<string,string>::iterator it;
    it = m_tags.find(url);
    if (it == m_tags.end()) {
        m_next_id += 1;
        string tag = baseN(m_next_id);
        m_tags[url] = tag;
        m_urls[tag] = url;
        return m_root + tag;
    }
    return m_root + m_urls[url];
}
