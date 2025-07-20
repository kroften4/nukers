#ifndef _UTILITY_H
#define _UTILITY_H

#include <stddef.h>

static inline void remove_element_by_index(void *objects[], size_t *n, size_t index) {
    *n -= 1;
    for (size_t i = index; i < *n; i++) {
        objects[i] = objects[i + 1];
    }
}

#endif

