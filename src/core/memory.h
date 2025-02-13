#pragma once

typedef enum {
    MEMORY_CATEGORY_VULKAN,
    MEMORY_CATEGORY_GAME,
    MEMORY_CATEGORY_SCENEGRAPH,
    MEMORY_CATEGORY_COUNT // number of memory categories
} MemoryCategory;

// define a structure that tracks allocated pointer, its category and size
typedef struct {
    MemoryCategory category; // memory category of this block
    size_t allocation_size;  // size of the allocation requested by the user
} Opointer;

void *oalloc(size_t size, MemoryCategory category);
void ofree(void *pointer);
size_t get_allocated_memory(MemoryCategory category);
size_t get_total_allocated_memory();
void log_memory_report();
void memory_cleanup();