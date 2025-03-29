#ifndef HASHMAP_STR_STR
#define HASHMAP_STR_STR

#include "../strings/dynamic_str/String.h"
#include "../vector/VectorStr.h"

typedef struct HashMapValueNode {
    String value;
    struct HashMapValueNode* next;
} HashMapValueNode;

typedef struct {
    VectorStr keys;
    HashMapValueNode** value_nodes;
    int capacity;
    int size;
} HashMapStrStr;

unsigned int hash(String str) {

}

void construct_hash_map_str_str(HashMapStrStr* map) {
    map->size = 0;
    map->capacity = 2;
    construct_vector_str(&map->keys, 2);
    map->value_nodes = (HashMapValueNode**)allocate(map->capacity * sizeof(HashMapValueNode*));
}

void add_in_hash_map_str_str(HashMapStrStr* map, String key, String value) {
    add_in_vector_str_by_moving(&map->keys, key);

    unsigned int position = hash(key) % map->capacity;

    
}

#endif