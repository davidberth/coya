#pragma once

typedef enum {
    MEMORY_CATEGORY_VULKAN,
    MEMORY_CATEGORY_GAME,
    MEMORY_CATEGORY_SCENEGRAPH,
    MEMORY_CATEGORY_TEXTURE,
    MEMORY_CATEGORY_STRING,
    MEMORY_CATEGORY_COUNT // number of memory categories
} MemoryCategory;

// define a structure that tracks allocated pointer, its category and size
typedef struct OpointerNode {
    MemoryCategory category; // memory category of this block
    size_t allocation_size;  // size of the allocation requested by the user
#ifndef _DEBUGGG
    const char *file;          // source file where allocation occurred
    int line;                  // line number where allocation occurred
    struct OpointerNode *next; // linked list of active allocations
    void *user_ptr;            // pointer returned to user
#endif
} Opointer;

// internal functions
void *oalloc_debug(
  size_t size, MemoryCategory category, const char *file, int line);
void *oalloc_release(size_t size, MemoryCategory category);
void ofree(void *pointer);
size_t get_allocated_memory(MemoryCategory category);
size_t get_total_allocated_memory();
void log_memory_report();
void memory_cleanup();

// macro to automatically capture debug info
#ifndef _DEBUGGG
#define oalloc(size, category) oalloc_debug(size, category, __FILE__, __LINE__)
#else
#define oalloc(size, category) oalloc_release(size, category)
#endif