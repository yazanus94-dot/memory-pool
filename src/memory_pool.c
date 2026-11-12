/**
 * Memory Pool - Implementation
 */

#include "memory_pool.h"
#include <stdlib.h>
#include <string.h>

MemoryPool *memory_pool_create(size_t block_size, size_t pool_size) {
    if (block_size == 0 || pool_size == 0) {
        return NULL;
    }

    // Ensure block_size is at least large enough to hold a pointer
    // for the free list linking
    if (block_size < sizeof(void *)) {
        block_size = sizeof(void *);
    }

    MemoryPool *pool = (MemoryPool *)malloc(sizeof(MemoryPool));
    if (!pool) return NULL;

    pool->block_size = block_size;
    pool->pool_size = pool_size;
    pool->free_count = pool_size;
    pool->memory = (uint8_t *)malloc(block_size * pool_size);
    if (!pool->memory) {
        free(pool);
        return NULL;
    }

    // Initialize free list - link all blocks together
    pool->free_list = NULL;
    uint8_t *ptr = pool->memory;
    for (size_t i = 0; i < pool_size; i++) {
        // Each free block points to the next free block
        void **block = (void **)ptr;
        *block = pool->free_list;
        pool->free_list = ptr;
        ptr += block_size;
    }

    return pool;
}

void *memory_pool_alloc(MemoryPool *pool) {
    if (!pool || pool->free_count == 0) {
        return NULL;
    }

    // Pop from the free list
    void *block = pool->free_list;
    pool->free_list = *(void **)block;  // Next free block
    pool->free_count--;

    return block;
}

void memory_pool_free(MemoryPool *pool, void *ptr) {
    if (!pool || !ptr) return;

    // Push back onto the free list
    *(void **)ptr = pool->free_list;
    pool->free_list = ptr;
    pool->free_count++;
}

void memory_pool_destroy(MemoryPool *pool) {
    if (!pool) return;
    free(pool->memory);
    free(pool);
}

size_t memory_pool_free_count(const MemoryPool *pool) {
    return pool ? pool->free_count : 0;
}

size_t memory_pool_capacity(const MemoryPool *pool) {
    return pool ? pool->pool_size : 0;
}

// Arena implementation
Arena *arena_create(size_t size) {
    if (size == 0) return NULL;
    Arena *arena = (Arena *)malloc(sizeof(Arena));
    if (!arena) return NULL;
    arena->memory = (uint8_t *)malloc(size);
    if (!arena->memory) {
        free(arena);
        return NULL;
    }
    arena->size = size;
    arena->offset = 0;
    return arena;
}

void *arena_alloc(Arena *arena, size_t size) {
    if (!arena || size == 0) return NULL;
    
    // Align to 8 bytes
    size_t aligned = (size + 7) & ~7;
    
    if (arena->offset + aligned > arena->size) {
        return NULL;  // Out of memory
    }
    
    void *ptr = arena->memory + arena->offset;
    arena->offset += aligned;
    return ptr;
}

void arena_reset(Arena *arena) {
    if (arena) {
        arena->offset = 0;
    }
}

void arena_destroy(Arena *arena) {
    if (!arena) return;
    free(arena->memory);
    free(arena);
}