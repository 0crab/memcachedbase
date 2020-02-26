
#include "memcached.h"

static libcuckoo_hash_table *hashTable;

void cuckoo_init(){
    hashTable=libcuckoo_hash_table_init(0);
}

item * cuckoo_find(uint64_t *key){
    item *it;
    if(libcuckoo_hash_table_find(hashTable,key,&it)){
        return it;
    }
    return NULL;
}

int cuckoo_insert(item *item){
    uint64_t *key=ITEM_key(item);
    libcuckoo_hash_table_insert(hashTable,key,&item);
    return 1;
}

void cuckoo_delete(uint64_t *key){
    libcuckoo_hash_table_erase(hashTable,key);
}