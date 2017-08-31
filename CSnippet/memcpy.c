#include <stdio.h>
#include <string.h>

void *my_memcpy(void *dst, const void *src, size_t size) {

    char *d;
    char *s;
    int i;

    // 处理内存重叠
    // |-------src---------|
    //              |----dst-----------|
    if ( (src < dst) && ((char *)src + size > (char *)dst)) {
        d = (char *)dst + size - 1;
        s = (char *)src + size - 1;
        while (size--) {
            *d-- = *s--;
        }
    } else {
        d = (char *)dst;
        s = (char *)src;
        while(size--){
            *d++ = *s++;
        }
    }
    return dst;
}

int main()
{
    char* s="GoldenGlobalView";
    char d[20];
    my_memcpy(d,s,(strlen(s)+1));
    printf("%s\n",d);
    return 0;
}
