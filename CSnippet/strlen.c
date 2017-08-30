#include <assert.h>
#include <stdio.h>

int strlen(const char *p) {
    assert(p != NULL);
    int len = 0;
    while ( *p++ ) len++;
    return len;
}

int main() {
    char *m = "hello";
    printf("%d\n", strlen(m));
    return 0;
}
