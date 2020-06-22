/* 
 * File:   HashTable.h
 * Author: usamanwar
 *
 * Created on September 25, 2013, 4:21 PM
 */

#ifndef HASHTABLE_H
#define	HASHTABLE_H

#include <stdint.h>
#define INS_HT_MAX_SIZE 10000


struct ht_node {
  uint32_t val;
  uint64_t key;
  struct ht_node *next;
};

struct ht_return {
  uint32_t val;
  uint32_t flag;
};

class HashTable {
public:
    static inline uint64_t hash(uint64_t hashval);
    static void dump_table(HashTable *ht);
    static inline uint64_t indexof(uint64_t hashval,uint64_t size); 
    static HashTable *ht_create(uint64_t size);                	/* allocate hash table memory*/
    static void* ht_destroy(HashTable *ht);                	/* free hash table memory*/
    static struct ht_return * ht_get(HashTable *ht,uint64_t key);      /* retrieve entry */
    static void ht_put(HashTable *ht,uint64_t key,uint32_t val);   	/* store entry */
    static void ht_remove(HashTable *ht,uint64_t key);         	/* remove entry */
    HashTable();
    HashTable(const HashTable& orig);
    virtual ~HashTable();
private:
    struct ht_node **table;
    uint32_t size;
};

#endif	/* HASHTABLE_H */

