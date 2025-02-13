#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"

// global array to track total bytes allocated per category
static size_t memory_totals[MEMORY_CATEGORY_COUNT] = {0};

// allocate memory and store header before user pointer
void *oalloc(size_t size, MemoryCategory category) {
    // allocate memory for header and user data
    size_t total_size = sizeof(Opointer) + size;
    // allocate block; note: for simplicity we do not align beyond malloc
    // guarantees
    void *block = malloc(total_size);
    if (!block) {
        // allocation failed so return nullptr
        return nullptr;
    }
    // set header values
    Opointer *opointer_ptr = (Opointer *)block;
    opointer_ptr->category = category;
    opointer_ptr->allocation_size = size;
    // track allocation in our total
    memory_totals[category] += size;
    // return pointer after the header to user
    return (void *)((char *)block + sizeof(Opointer));
}

// free memory previously allocated via custom_alloc
void ofree(void *pointer) {
    if (!pointer) {
        // nothing to free
        return;
    }
    // get header pointer stored before user pointer
    Opointer *opointer_ptr = (Opointer *)((char *)pointer - sizeof(Opointer));
    // update allocation tracking
    memory_totals[opointer_ptr->category] -= opointer_ptr->allocation_size;
    // free the entire block
    free((void *)opointer_ptr);
}

// get currently allocated memory for a given category
size_t get_allocated_memory(MemoryCategory category) {
    return memory_totals[category];
}

// get total allocated memory for all categories
size_t get_total_allocated_memory() {
    size_t total = 0;
    for (int i = 0; i < MEMORY_CATEGORY_COUNT; ++i) {
        total += memory_totals[i];
    }
    return total;
}
