#include "shortUrl.h"
#include <iostream>

int main () {
    Shortener surl;
    string url;
    cout << "enter your origin URL:";
    cin >> url;
    cout << "Shorten URL:";
    cout << surl.shorten(url);
    return 0;
}
