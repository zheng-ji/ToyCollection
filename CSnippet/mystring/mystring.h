#ifndef MSTRING_H
#define MSTRING_H

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct Mystring {
    int size;
    int capacity;
    char *data;
} Mystring;

enum {
    E_OK = 0,
    E_ONMALLOC = -1,
    E_EMPTYMSTRING = -2,
    E_RESIZE = -3,
};


int destroyMystring(Mystring *s);
int capOfMystring(Mystring *s);
int sizeOfMystring(Mystring *s);
int appendMystring(Mystring *s, const char *str);

Mystring *newMystring(const char *str);
Mystring *subMystring(const Mystring * s, int from, int to);
Mystring *copyMystring(const Mystring *s);
Mystring *mergeMystring(const Mystring *first, const Mystring *second);

char *strMystring(const Mystring *s, char c);
char *atMystring(const Mystring*s , int index);

#if defined(__cplusplus)
}
#endif

#endif
