#include "mystring.h"
#include <stdlib.h>
#include <string.h>

const int DEFAULT_CAPACITY = 10;
static int resize(Mystring *s, int capacity);
//int resize(Mystring *s, int capacity);

Mystring *newMystring(const char *str) {

    Mystring * mstring = (Mystring *)malloc(sizeof(Mystring));
    if (mstring == NULL) {
        return NULL;
    }


    int size = (str == NULL) ? 0: strlen(str);
    mstring->size = size;

    int capacity = (size > DEFAULT_CAPACITY) ? size: DEFAULT_CAPACITY;
    mstring->capacity = capacity;

    char * s = (char *)malloc(sizeof(char) * capacity+1);
    if (s == NULL) {
        free(mstring);
        return NULL;
    }
    mstring->data = s;
    memcpy(mstring->data, str, mstring->size);
    mstring->data[mstring->size] = '\0';
    return mstring;
}


int resize(Mystring *s, int least_capacity) {
    int capacity = (s->capacity * 2 > least_capacity) ? s->capacity*2: least_capacity;
    char * data = (char *)malloc(sizeof(char ) * (capacity + 1));
    if (data == NULL) {
        return E_ONMALLOC;
    }
    memcpy(data, s->data, s->size);
    data[s->size] = '\0';

    free(s->data);
    s->data = data;
    s->capacity = capacity;
    return E_OK;
}

int appendMystring(Mystring *s, const char *str) {

    if (s == NULL) {
        return E_EMPTYMSTRING;
    }

    if ((s->size + strlen(str)) > s->capacity) {
        if (resize(s, s->size+strlen(str) != E_OK)) {
            return E_RESIZE;
        }
    }

    strncat(s->data, str, strlen(str));
    s->size += strlen(str);
    return E_OK;
}

int destroyMystring(Mystring *s) {
    if (s != NULL) {
        if (s->data != NULL) {
            free(s->data);
        }
        free(s);
    }
    return E_OK;
}

int capOfMystring(Mystring *s) {
    return (s != NULL)? s->capacity: 0;
}

int sizeOfMystring(Mystring *s) {
    return (s != NULL)? s->size: 0;
}

Mystring *subMystring(const Mystring *s, int from, int to) {
    if (to < from) 
        return NULL;
    int size = to - from;
    char *data = (char *)malloc(sizeof(char)*(size+1));
    if (data == NULL) {
        return NULL;
    }
    memcpy(data, s->data+from, size);
    data[size] = '\0';

    Mystring * tmp = newMystring(data);
    free(data);
    return tmp;
}

Mystring *copyMystring(const Mystring *s) {
    if (s == NULL) return NULL;
    Mystring *new_mstring = (Mystring *)malloc(sizeof(Mystring));
    if (new_mstring == NULL) return NULL;

    new_mstring->capacity = s->capacity;
    new_mstring->size = s->size;
    new_mstring->data = (char *)malloc(sizeof(char)*(s->size+1)); 
    if (new_mstring->data == NULL) {
        free(new_mstring);
        return NULL;
    }
    memcpy(new_mstring->data, s->data, s->size);
    new_mstring->data[s->size] = '\0';
    return new_mstring;
}

Mystring *mergeMystring(const Mystring *first, const Mystring *second) {
    if (first == NULL  && second == NULL) {
        return NULL;
    }
    else if (first != NULL && second != NULL) {
        Mystring *s = copyMystring(first);
        if (s == NULL) {
            return NULL;
        }
        int status = appendMystring(s, second->data);
        //if (status != E_OK) {
        if (status != 0) {
            destroyMystring(s);
        }
        return s;
    }
    const Mystring *s= (first != NULL)? first: second;
    return copyMystring(s);
}

char * strMystring(const Mystring *s, char c) {
    if (s == NULL || s->data == NULL) {
        return NULL;
    }

    char * p = s->data;
    while (*p != '\0' && *p != c) {
        p++;
    }
    if (*p != '\0') {
        return p;
    } else {
        return NULL;
    }
}

char *atMystring(const Mystring*s, int index) {
    if (s == NULL || index > s->size) {
        return NULL;
    }
    return s->data+index;
}
