#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdint.h>

typedef struct Node Node_t;

struct Node
{
    void* data;
    Node_t* next;
};

Node_t* create_node();
uint8_t add(void* data, Node_t* root);
uint8_t remove(Node_t* node, Node_t* root);

#endif
