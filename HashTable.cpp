/* 
 * File:   HashTable.cpp
 * Author: usamanwar
 * 
 * Created on September 25, 2013, 4:21 PM
 */

#include "HashTable.h"
#include <stdlib.h>
#include <stdio.h>

HashTable::HashTable() {
}

HashTable::HashTable(const HashTable& orig) {
}

HashTable::~HashTable() {
}

inline uint64_t HashTable::hash(uint64_t hashval) {
    
    hashval = ((hashval >> 17) ^ hashval);
    hashval = ((hashval >> 8) ^ hashval);
    return hashval;
    
}
inline uint64_t HashTable::indexof(uint64_t hashval,uint64_t size) {
    return (hashval % size);
}
 /* allocate hash table memory*/
HashTable * HashTable::ht_create(uint64_t size) {
     HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
     ht->size = size;
     ht->table = (ht_node **)malloc(size * sizeof(struct ht_node *)); 
     //printf("\nHashmap created of size = %d:\n",size);
     return ht;
}
 /* print all table values */
void HashTable::dump_table(HashTable *ht) {
    if (!ht) return;
    printf("\nDUMP:Table\n");
    uint64_t i =0,j=0;
    struct ht_node *n;
    for(i = 0 ; i < ht->size ; i++ )
    {
        n = ht->table[i];
        while( n != NULL )
        {
                printf("key#%llx=0x%llx\tval=0x%lx\n",++j,n->key,n->val);
                n = n->next;        
        }
        printf("*********index=%llx*******\n",i);
    }
    printf("Total Hash values:%ld\n",j);
}
/* free hash table memory*/
void* HashTable::ht_destroy(HashTable *ht) {
        if (ht == NULL) return NULL;
        uint64_t i=0;
        for (i = 0  ; i < ht->size; i++) 
        {
                struct ht_node *n = ht->table[i];
                while (n != NULL) 
                {
                        struct ht_node *n_old = n;
                        n = n->next;
                        free(n_old); 
                        n_old = NULL;
                       
                }
        }
        free(ht->table);
        ht->table = NULL;
        free(ht);
        ht = NULL;
        return NULL;
}
/* retrieve entry */
struct ht_return * HashTable::ht_get(HashTable *ht,uint64_t key) {
	struct ht_return * ret = (ht_return *)malloc(sizeof(struct ht_return));
        ret->flag = 0;
	if (!ht)
	{ 
              return ret;
	}
        struct ht_node *n = ht->table[indexof(hash(key),ht->size)];
        while(n != NULL)
        {
            if(n->key == key)
	    {	
		ret->flag = 1;
		ret->val = n->val;
		return ret;
	    }
            n = n->next;
        }
        return ret;
}
/* store entry */
void HashTable::ht_put(HashTable *ht,uint64_t key,uint32_t val) {
        if (!ht) return;
        uint64_t idx = indexof(hash(key),ht->size);
        struct ht_node *n_new = (ht_node *)malloc(sizeof(struct ht_node));
        n_new->key = key;
        n_new->val = val;
        n_new->next = ht->table[idx];
        ht->table[idx] = n_new;
}
/* remove entry */
void HashTable::ht_remove(HashTable *ht,uint64_t key) {
    if (!ht) return;
    uint64_t idx = indexof(hash(key),ht->size);
    struct ht_node *prev = NULL;
    struct ht_node *next = ht->table[idx];
    if(next == NULL) 
        return;
    else 
        prev = next;
    while (next != NULL && next->key != key) {
        prev = next;
        next = next->next;
    }
    if(next == NULL) return;
    else if(prev == next)
        ht->table[idx] = next->next;
    else
        prev->next = next->next;
}

