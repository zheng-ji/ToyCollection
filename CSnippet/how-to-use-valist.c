#include <stdio.h>
#include <stdarg.h>
#include <string.h>

/*
void va_start( va_list arg_ptr, prev_param ); 
type va_arg( va_list arg_ptr, type ); 
void va_end( va_list arg_ptr );
*/

void read_num(int num, ...) {
    va_list va;
    va_start(va, num);
    while (num--) {
        printf("%d\t", va_arg(va, int));
    }
    va_end(va);
}

void write_log(char *fmt, ...) {
    va_list va;
    va_start(va, fmt);
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    vsprintf(buffer, fmt, va);
    va_end(va);
    printf("%s-%s", "my-log-prehead", buffer);
}

int main() {
    write_log("%s\n", "hello-world");
    read_num(2, 3, 5);
}
