#include<iostream>

using namespace std;

void reverse(char *s) {
    if (*s != '\0') {
        reverse(s+1);
        cout << *s;
    } else {
        cout << endl;
    }
}

int main() {
    char str[100];
    cin >> str;
    reverse(str);
    return 0;
}
