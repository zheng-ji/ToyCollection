#include <stdio.h>
#include <assert.h>

int strcmp(const char* dst, const char* src) {
    assert( dst != NULL && src != NULL);

    while ( *dst == *src) {
        if (*dst == '\0') return 0;
        dst ++;
        src ++;
    }
    return *dst - *src;
}

int main() {
    char *s1 = "hello";
    char *s2 = "hillo";
    printf("%d", strcmp(s1,s2));
}
