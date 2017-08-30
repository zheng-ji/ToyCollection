#include<stdio.h>
#include<assert.h>

char *strcat(char *dest, const char *src) {
    assert((dest != NULL) && (src != NULL));
    char *address = dest;
    while(*dest != '\0') dest ++;
    while(*dest++ = *src++);
    return address;
}

int main() {
    char dest[] = "ok"; 
    //char *dest = "ok"; 这个是会失败,因为常量字符串的内容不能修改
    char *src = "world";

    printf("%s\n", strcat(dest, src));
}
