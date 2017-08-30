#include <stdio.h>
#include <assert.h>

char * strcpy(char *dst, const char *src) {
    assert((dst != NULL) && (src != NULL));

    char * ret = dst;
    while((*dst++ = *src++) != '\0');
    return ret;
}

int main() {
    char dst[] = "ob";
    char *src = "hello";
    printf("%s\n", strcpy(dst, src));
    return 0;
}
