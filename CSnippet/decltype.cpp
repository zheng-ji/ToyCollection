#include <iostream>
#include <string>
#include <cctype>
using namespace std;

int main () {
    string str("some, good, day!!");
    decltype(str.size()) punct_num = 0;
    for (auto &c: str) {
        if (ispunct(c)) {
            punct_num += 1;
        } else {
            c = toupper(c);
        }
    }
    cout << str << endl;
    cout << punct_num << endl;

    const string s = "keep out!";
    for (auto &c : s) {
        cout << c << endl;
    }
    return 0;
}
