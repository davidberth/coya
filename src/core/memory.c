#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/logger.h"
#include "memory.h"

// global array to track total bytes allocated per category
static size_t memory_totals[MEMORY_CATEGORY_COUNT] = {0};

#ifdef _DEBUG
// head of active allocations list
static Opointer *active_allocations = nullptr;
#endif

static const char *memory_category_names[] = {
  "Vulkan",
  "Game",
  "SceneGraph",
  "String",
};

// debug version of allocation that stores file and line info
#if defined(_DEBUG)
void *oalloc_debug(
  size_t size, MemoryCategory category, const char *file, int line) {
    mlog("allocating %zu bytes in category %s at %s:%d", size,
      memory_category_names[category], file, line);
    size_t total_size = sizeof(Opointer) + size;

    void *block = malloc(total_size);
    if (!block) {
        return nullptr;
    }

    Opointer *opointer_ptr = (Opointer *)block;
    opointer_ptr->category = category;
    opointer_ptr->allocation_size = size;
    opointer_ptr->file = file;
    opointer_ptr->line = line;
    void *user_ptr = (void *)((char *)block + sizeof(Opointer));
    opointer_ptr->user_ptr = user_ptr;

    // add to active allocations list
    opointer_ptr->next = active_allocations;
    active_allocations = opointer_ptr;

    memory_totals[category] += size;
    return user_ptr;
}
#endif

#if defined(NDEBUG)
// release version of allocation without debug info
void *oalloc_release(size_t size, MemoryCategory category) {
    mlog("+ allocating %zu bytes in category %s", size,
      memory_category_names[category]);
    size_t total_size = sizeof(Opointer) + size;

    void *block = malloc(total_size);
    if (!block) {
        return nullptr;
    }

    Opointer *opointer_ptr = (Opointer *)block;
    opointer_ptr->category = category;
    opointer_ptr->allocation_size = size;
    memory_totals[category] += size;

    return (void *)((char *)block + sizeof(Opointer));
}
#endif

// free memory previously allocated
void ofree(void *pointer) {
    if (!pointer) {
        return;
    }
    Opointer *opointer_ptr = (Opointer *)((char *)pointer - sizeof(Opointer));
    memory_totals[opointer_ptr->category] -= opointer_ptr->allocation_size;

#ifdef _DEBUG
    // remove from active allocations list
    if (active_allocations == opointer_ptr) {
        active_allocations = opointer_ptr->next;
    } else {
        Opointer *curr = active_allocations;
        while (curr && curr->next != opointer_ptr) {
            curr = curr->next;
        }
        if (curr) {
            curr->next = opointer_ptr->next;
        }
    }
#endif

    mlog("- freeing %zu bytes in category %s", opointer_ptr->allocation_size,
      memory_category_names[opointer_ptr->category]);
    free((void *)opointer_ptr);
}

size_t get_allocated_memory(MemoryCategory category) {
    return memory_totals[category];
}

size_t get_total_allocated_memory() {
    size_t total = 0;
    for (int i = 0; i < MEMORY_CATEGORY_COUNT; ++i) {
        total += memory_totals[i];
    }
    return total;
}

void log_memory_report() {
    mlog("memory report:");
    for (int i = 0; i < MEMORY_CATEGORY_COUNT; ++i) {
        mlog(" %-12s: %zu bytes", memory_category_names[i], memory_totals[i]);
    }
    mlog(" Total       : %zu bytes", get_total_allocated_memory());
}

#ifdef _DEBUG
// debug version of cleanup that reports leak locations
void memory_cleanup() {
    mlog("checking for memory leaks");
    log_memory_report();
    size_t total = get_total_allocated_memory();
    if (total > 0) {
        elog("Memory leak detected! %zu bytes not freed", total);
        // report each leaked allocation
        Opointer *curr = active_allocations;
        while (curr) {
            elog("  Leak of %zu bytes in %s at %s:%d", curr->allocation_size,
              memory_category_names[curr->category], curr->file, curr->line);
            curr = curr->next;
        }
    } else {
        mlog("no memory leaks detected");
    }
}
#else
// release version of cleanup
void memory_cleanup() {
    log_memory_report();
    if (get_total_allocated_memory() > 0) {
        elog("Memory leak detected!");
    }
}
#endif