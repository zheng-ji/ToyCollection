#include <string.h>
#include <iostream>
#include "hash.h"
using namespace std;

Hashtab::Hashtab() {
	inithashtab();
}

void Hashtab::inithashtab() {
	for (int i = 0; i < HASHSIZE;i++)
		hashtab[i] = NULL;
}

unsigned int Hashtab::hash(char * s) {
	unsigned int h = 0;
	for (; *s;s++) {
		h = *s + h * 31;
	}
	return h % HASHSIZE;
}

node * Hashtab::lookup(char * n) {
	unsigned int hi = hash(n);
	node * np = hashtab[hi];
	for (;np != NULL;np = np ->next) {
		if(!strcmp(np->name,n))
			return np;
	}
	return NULL;
}


char * Hashtab::m_strdup(char * o) {
	int l = strlen(o) + 1;
	char * ns = new char[l];
	strcpy(ns,o);
	if (ns == NULL)
		return NULL;
	else 
		return ns;
}

char * Hashtab::get(char * name) {
	node * n = lookup(name);
	if (n == NULL)
		return NULL;
	else
		return n->desc;
}

int Hashtab::install(char * name,char * desc) {
	unsigned int hi;
	node * np;
	if (lookup(name) == NULL) {
		hi = hash(name);
		np = new node;
		if (np == NULL) return 0;
		np -> name = m_strdup(name);
		if (np ->name == NULL) return 0;
        np ->desc = m_strdup(desc);
		np -> next = NULL;
		hashtab[hi] = np;
	} else {
		hi = hash(name);
		np = new node;
		if (np == NULL) return 0;
        np ->name = m_strdup(name);
        np ->desc = m_strdup(desc);
        np -> next = NULL;
        node *tmp = hashtab[hi];
        for (;tmp->next != NULL; tmp = tmp->next) ;
        tmp->next = np;
    }
	if (np -> desc == NULL)
		return 0;
	return 1;
}

void Hashtab::displaytable() {
	int i;
	node * t;
	for (i = 0; i < HASHSIZE;i++) {
		if (hashtab[i] == NULL)
			cout << "()" << endl;
		else {
			t = hashtab[i];
			cout << "(";
			for (; t != NULL; t = t -> next) {
				cout << "(" << t->name << "." << t->desc << ")";
			}
			cout << ")" << endl;
		}
	}
}

void Hashtab::cleanup() {
	node * np,*t;
	for (int i = 0; i < HASHSIZE;i++) {
		if (hashtab[i] != NULL) {
			np = hashtab[i];
			while (np != NULL) {
				t = np ->next;
				delete np->name;
				delete np->desc;
				delete np;
				np = t;
			}
		}
	}
}

Hashtab::~Hashtab() {
	cleanup();
}

int main (int argc, char * argv[]) {
	Hashtab ht;
	ht.inithashtab();

	char* names[]= {"name","address","phone","k101","k110"};
    char* descs[]= {"Sourav","Sinagor","26300788","Value1","Value2"};

    for(int i = 0; i < 5; i++)
        ht.install(names[i],descs[i]);

    cout << "Done" << endl;
    cout << ht.get("k110") << endl;

    ht.install("phone","9433120451");
    ht.install("phone","9430451");

	cout << ht.get("k101") << " " << ht.get("phone") << endl;

	ht.displaytable();
	return 0;
} 


