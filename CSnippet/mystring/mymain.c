#include "mystring.h"
#include <stdio.h>

void print_String(const Mystring *s)
{
    printf("string: \n\tcontent:%s\n\tsize:%d\n\tcapacity: %d\n\n", s->data, s->size, s->capacity);
}

int main(void)
{ 
    Mystring *str = newMystring("Hello");
    print_String(str);

    Mystring *other = copyMystring(str);
    print_String(other);

    appendMystring(str, ", World!");
    print_String(str);

    for (int i = 0; i < 10; i++)
    {
        appendMystring(str, "Hello, World");
        print_String(str);
    }

    destroyMystring(str);
    destroyMystring(other);

    return 0;
}
