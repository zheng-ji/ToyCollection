/** 
 * @file hash.cpp
 * @brief 
 * @author zhengji - zhengji91@gmail.com
 * @version 
 * @date 2013-05-13
 */
#ifndef _HASH_H_
#define _HASH_H_

#define HASHSIZE 101

typedef struct _node {
	char * name;
	char * desc;
	struct _node * next;
} node;

class Hashtab {
	public:

		Hashtab();
		~Hashtab();
		void inithashtab();
		unsigned int hash(char * s);
		node *lookup(char * s);
		char * m_strdup(char *o);
		int install(char * name,char * desc);
		char *get(char * name);
		void displaytable();
		void cleanup();
		
	private:
		node * hashtab[HASHSIZE];
};

#endif
