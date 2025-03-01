#include "../../inc/graphics/graphics-mem.h"
#include "../../inc/animations/animations.h"
#include "../../inc/collections/linked-list.h"
#include "../../inc/utils/utils.h"
#include "../../inc/status.h"

static Node_t* animations_list;

uint8_t init_graphics_scheduler() {
    animations_list = create_node();
    if (animations_list == (NULL)) {
        return (ENOMEM);
    }

    return (OS_ALL_OK);
}

uint8_t apend_operation(Animation_t* animation) {
    return add(animation, animations_list);
}

uint8_t remove_operation(Node_t* operation_node) {
    return remove(operation_node, animations_list);
}