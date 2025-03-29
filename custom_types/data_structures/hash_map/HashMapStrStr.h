#ifndef HASHMAP_STR_STR
#define HASHMAP_STR_STR

#include "../strings/dynamic_str/String.h"
#include "../vector/VectorStr.h"

enum HashMapStrStrModeOfHandlingStringArgs {
    COPY,
    MOVE
};

typedef struct HashMapNode {
    String value;
    String key;
    struct HashMapNode* next;
} HashMapNode;

typedef struct {
    HashMapNode** nodes;
    int capacity;
    int size;
    enum HashMapStrStrModeOfHandlingStringArgs mode_of_handling_args;
} HashMapStrStr;

unsigned int hash_map_str_str_hash_djb2(const String* str) {
    unsigned int hash = 5381;
    for (int i = 0; i < str->size; ++i) {
        hash = ((hash << 5) + hash) + (unsigned char)str->symbol_sequence[i]; 
    }
    return hash;
}

void construct_hash_map_str_str(HashMapStrStr* map) {
    map->size = 0;
    map->capacity = 2;
    map->mode_of_handling_args = MOVE;
    map->nodes = (HashMapNode**)allocate(map->capacity * sizeof(HashMapNode*));

    for (int i = 0; i < map->capacity; ++i) {
        map->nodes[i] = 0;
    }
}

void resize_hash_map_str_str(HashMapStrStr* map) {

    //TODO: fix bugs

    int old_capacity = map->capacity;
    map->capacity *= 2;
    HashMapNode** nodes = (HashMapNode**)allocate(map->capacity * sizeof(HashMapNode*));

    for (int i = 0; i < old_capacity; ++i) {
        nodes[i] = map->nodes[i];
    }

    free(map->nodes);
    map->nodes = nodes;
}

void extend_map_str_str_if_necessery(HashMapStrStr* map) {
    if (map->size < map->capacity / 2) {
        return;
    }

    resize_hash_map_str_str(map);
}

void add_in_hash_map_str_str_COPY(HashMapStrStr* map, String key, String value) {
    extend_map_str_str_if_necessery(map);

    unsigned int position = hash_map_str_str_hash_djb2(&key) % map->capacity;
    
    HashMapNode* ptr = map->nodes[position];

    while (ptr) {
        if (strings_are_equal(&ptr->key, &key)) {
            free_string(&ptr->value);
            ptr->value = make_deep_copy(&value);
            return;
        }
        ptr = ptr->next;
    }

    HashMapNode* new_node = (HashMapNode*)allocate(sizeof(HashMapNode));
    new_node->key = make_deep_copy(&key);
    new_node->value = make_deep_copy(&value);
    new_node->next = 0;

    map->nodes[position] = new_node;

    map->size++;
}

void add_in_hash_map_str_str_MOVE(HashMapStrStr* map, String key, String value) {
    extend_map_str_str_if_necessery(map);

    unsigned int position = hash_map_str_str_hash_djb2(&key) % map->capacity;
    
    HashMapNode* ptr = map->nodes[position];

    while (ptr) {
        if (strings_are_equal(&ptr->key, &key)) {
            free_string(&ptr->value);
            ptr->value = value;
            return;
        }
        ptr = ptr->next;
    }

    HashMapNode* new_node = (HashMapNode*)allocate(sizeof(HashMapNode));
    new_node->key = key;
    new_node->value = value;
    new_node->next = 0;

    map->nodes[position] = new_node;

    map->size++;
}

void add_in_hash_map_str_str(HashMapStrStr* map, String key, String value) {
    switch (map->mode_of_handling_args) 
    {
    case COPY:
        return add_in_hash_map_str_str_COPY(map, key, value);
    case MOVE:
        return add_in_hash_map_str_str_MOVE(map, key, value);
    default:
        return;
    }
}

void free_hash_map_value_node(HashMapNode* node) {
    if (!node) {
        return;
    }

    free_string(&node->key);
    free_string(&node->value);
    
    free_hash_map_value_node(node->next);
}

void free_hash_map_str_str(HashMapStrStr* map) {
    for (int i = 0; i < map->capacity; ++i) {
        free_hash_map_value_node(map->nodes[i]);
    }
    free(map->nodes);
    map->capacity = 0;
    map->size = 0;
}

String get_in_hash_map_str_str(const HashMapStrStr* map, String key) {
    unsigned int position = hash_map_str_str_hash_djb2(&key) % map->capacity;

    HashMapNode* ptr = map->nodes[position];
    while (ptr && !strings_are_equal(&ptr->key, &key)) {
        ptr = ptr->next;
    }

    if (ptr) {
        return ptr->value;
    }

    return string_from("");
}

void hash_map_str_str_foreach(const HashMapStrStr* map, void(*action)(String*)) {
    for (int i = 0; i < map->capacity; ++i) {
        if (map->nodes[i] == 0) {
            continue;
        }
        action(&map->nodes[i]->value);

        HashMapNode* ptr = map->nodes[i];
        while(ptr->next) {
            action(&ptr->value);
        }
    }
}

#endif