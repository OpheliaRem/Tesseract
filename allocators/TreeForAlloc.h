#ifndef TREE_FOR_ALLOC
#define TREE_FOR_ALLOC

#include "linear_allocator.h"
#include "../boolean.h"

typedef struct {
    void* address;
    int capacity;
} AllocationUnit;

typedef struct AllocationTreeNode {
    AllocationUnit unit;
    struct AllocationTreeNode* right;
    struct AllocationTreeNode* left;
    struct AllocationTreeNode* parent;
} AllocationTreeNode;

typedef struct {
    AllocationTreeNode* array;
    AllocationTreeNode* root;
    int size;
    int capacity;
} AllocatorBinaryTree;

void construct_alloc_tree(AllocatorBinaryTree* tree, const int capacity) {
    tree->size = 0;
    tree->capacity = capacity;
    tree->array = (AllocationTreeNode*)allocate_linear(tree->capacity);
    tree->root = tree->array;
}

boolean alloc_tree_is_empty(const AllocatorBinaryTree* tree) {
    return tree->size <= 0;
}

void add_in_alloc_tree(AllocatorBinaryTree* tree, AllocationUnit unit) {    
    if (alloc_tree_is_empty(tree)) {
        tree->size++;
        tree->root->unit = unit;
        tree->root->left = 0;
        tree->root->right = 0;
        tree->root->parent = 0;
        return;
    }

    tree->array[tree->size++].unit = unit;

    if (tree->capacity == tree->size) {
        tree->capacity *= 2;
        AllocationTreeNode* array = (AllocationTreeNode*)allocate_linear(tree->capacity);
        for (int i = 0; i < tree->size; ++i) {
            array[i] = tree->array[i];
        }
        tree->array = array;
    }

    AllocationTreeNode* ptr = tree->array;
    while (ptr != 0) {
        if (unit.address < ptr->unit.address) {
            if (ptr->left == 0) {
                ptr->left = &(tree->array[tree->size - 1]);
                ptr->left->parent = ptr;
                ptr = ptr->left;
                break;
            }
            ptr = ptr->left;
        } else {
            if (ptr->right == 0) {
                ptr->right = &(tree->array[tree->size - 1]);
                ptr->right->parent = ptr;
                ptr = ptr->right;
                break;
            }
            ptr = ptr->right;
        }
    }

    ptr->right = 0;
    ptr->left = 0;
}

AllocationTreeNode* get_min_in_alloc_tree(const AllocatorBinaryTree* tree) {
    AllocationTreeNode* ptr = tree->root;

    while(ptr->left) {
        ptr = ptr->left;
    }

    return ptr;
}

void clear_fields_of_removed_node(AllocationTreeNode* removed_node) {
    removed_node->left = 0;
    removed_node->right = 0;
    removed_node->parent = 0;
    removed_node->unit.address = 0;
    removed_node->unit.capacity = 0;
}

void remove_in_alloc_tree_ROOT(AllocatorBinaryTree* tree, AllocationTreeNode* root) {
    if (root->left == 0 && root->right == 0) {
        return clear_fields_of_removed_node(root);
    }

    if (root->left != 0 && root->right == 0) {
        root->left->parent = 0;
        tree->root = root->left;
        return clear_fields_of_removed_node(root);
    }

    if (root->left == 0 && root->right != 0) {
        root->right->parent = 0;
        tree->root = root->right;
        return clear_fields_of_removed_node(root);
    }

    AllocationTreeNode* ptr = root;

    while(ptr->right) {
        ptr = ptr->right;
    }

    AllocationTreeNode* last_left_node_in_seq = ptr->left;

    while(ptr != root) {
        ptr->left = ptr->parent->left;
        ptr = ptr->parent;
    }

    add_in_alloc_tree(tree, last_left_node_in_seq->unit);

    root->left->parent = root->right;
    root->right->parent = 0;
    tree->root = root->right;

    clear_fields_of_removed_node(root);    
}

void remove_in_alloc_tree_NO_DESCENDANTS(AllocatorBinaryTree* tree, AllocationTreeNode* node) {
    if (node->parent->left == node) {
        node->parent->left = 0;
        return clear_fields_of_removed_node(node);
    }

    node->parent->right = 0;
    clear_fields_of_removed_node(node);
}

void remove_in_alloc_tree_ONE_DESCENDANT(AllocatorBinaryTree* tree, AllocationTreeNode* node) {

    AllocationTreeNode* descendant = node->left == 0 ? node->right : node->left;

    if (node->parent->left == node) {
        node->parent->left = descendant;
        descendant->parent = node->parent;
        return clear_fields_of_removed_node(node);
    }

    node->parent->right = descendant;
    descendant->parent = node->parent;
    clear_fields_of_removed_node(node);
}

void remove_in_alloc_tree_TWO_DESCENDANTS(AllocatorBinaryTree* tree, AllocationTreeNode* node) {
    if (node->parent->left == node) {
        node->parent->left = node->right;    
    } else {
        node->parent->right = node->right;
    }

    AllocationTreeNode* ptr = node;

    while(ptr->right) {
        ptr = ptr->right;
    }

    AllocationTreeNode* last_left_node_in_seq = ptr->left;

    while(ptr != node) {
        ptr->left = ptr->parent->left;
        ptr = ptr->parent;
    }

    add_in_alloc_tree(tree, last_left_node_in_seq->unit);

    node->left->parent = node->right;
    node->right->parent = node->parent;

    clear_fields_of_removed_node(node);
}

void remove_in_alloc_tree(AllocatorBinaryTree* tree, AllocationTreeNode* node) {
    if (node->parent == 0) {
        return remove_in_alloc_tree_ROOT(tree, node);
    }

    if (node->left == 0 && node->right == 0) {
        return remove_in_alloc_tree_NO_DESCENDANTS(tree, node);
    }

    if (node->left != 0 ^ node->right != 0) {
        return remove_in_alloc_tree_ONE_DESCENDANT(tree, node);
    }

    return remove_in_alloc_tree_TWO_DESCENDANTS(tree, node);
}

//O(n)
void update_tree_alloc(AllocatorBinaryTree* tree) {
    AllocationTreeNode* array = (AllocationTreeNode*)allocate_linear(tree->capacity);

    int j = 0;
    for (int i = 0; i < tree->size; ++i) {
        if (tree->array[i].left == 0 && tree->array[i].right == 0 && tree->array[i].parent == 0) {
            continue;
        }

        array[j] = tree->array[i];
        ++j;
    }

    tree->array = array;
    tree->size = j;
}

AllocationTreeNode* alloc_tree_search_for_key(const AllocatorBinaryTree* tree, void* key) {
    AllocationTreeNode* ptr = tree->root;

    while(ptr) {
        if (ptr->unit.address == key) {
            return ptr;
        }

        if (key > ptr->unit.address) {
            ptr = ptr->right;
        }

        ptr = ptr->left;
    }

    return 0;
}

void alloc_tree_foreach(const AllocatorBinaryTree* tree, void(*action)(AllocationTreeNode*)) {
    AllocationTreeNode* ptr = tree->root;
    while (ptr->left) {
        ptr = ptr->left;
    }

    //TODO:finish
}

#endif