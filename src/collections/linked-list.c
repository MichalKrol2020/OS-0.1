#include "../../inc/collections/linked-list.h"
#include "../../inc/memory/heap/kheap.h"
#include "../../inc/utils/utils.h"
#include "../../inc/status.h"

Node_t* create_node() {
    void* addr = kmalloc(sizeof(Node_t));
    if (addr == (ZERO)) {
        return (NULL);
    }

    Node_t* node = (Node_t*) addr;
    node -> next = (NULL);

    return node;
}

uint8_t add(void* data, Node_t* root) {
    Node_t* new_node = create_node();
    if (new_node == (NULL)) {
        return (ENOMEM);
    }

    new_node -> data = data;
    Node_t* temp = root;
    while (temp -> next != (NULL)) {
        temp = temp -> next;
    }
    temp -> next = new_node;

    return (OS_ALL_OK);
}

uint8_t remove(Node_t* node, Node_t* root) {
    if (node == (NULL) || root == (NULL)) {
        return (EINVAL);
    }

    if (root == node) {
        Node_t* temp = root;
        root = temp -> next;
        kfree(temp);

        return (OS_ALL_OK);
    }

    Node_t* temp = root;
    while (node != (temp -> next)) {
        temp = temp -> next;
    }

    Node_t* to_delete = temp -> next;
    Node_t* next = to_delete -> next;
    temp -> next = next;
    kfree(to_delete);

    return (OS_ALL_OK);
}


