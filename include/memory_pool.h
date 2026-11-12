/**
 * Memory Pool - High-Performance Memory Allocation Library
 * 
 * A fixed-size memory pool allocator with O(1) allocation and deallocation.
 * Uses a free list for fast reuse of freed blocks and arena allocation
 * for bulk memory management.
 */

#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Fixed-size memory pool allocator.
 * Provides O(1) allocation and deallocation for fixed-size objects.
 */
typedef struct MemoryPool {
    size_t block_size;    /* Size of each block */
    size_t pool_size;     /* Total number of blocks */
    size_t free_count;    /* Number of free blocks */
    uint8_t *memory;      /* Raw memory arena */
    void *free_list;      /* Head of free list */
} MemoryPool;

/**
 * Creates a new memory pool.
 * 
 * @param block_size The size of each block in bytes
 * @param pool_size  The number of blocks in the pool
 * @return Pointer to the created pool, or NULL on failure
 */
MemoryPool *memory_pool_create(size_t block_size, size_t pool_size);

/**
 * Allocates a block from the pool.
 * 
 * @param pool The memory pool
 * @return Pointer to the allocated block, or NULL if pool is full
 */
void *memory_pool_alloc(MemoryPool *pool);

/**
 * Frees a block back to the pool.
 * 
 * @param pool The memory pool
 * @param ptr  The block to free
 */
void memory_pool_free(MemoryPool *pool, void *ptr);

/**
 * Destroys the memory pool and frees all memory.
 * 
 * @param pool The memory pool to destroy
 */
void memory_pool_destroy(MemoryPool *pool);

/**
 * Returns the number of free blocks in the pool.
 */
size_t memory_pool_free_count(const MemoryPool *pool);

/**
 * Returns the total capacity of the pool.
 */
size_t memory_pool_capacity(const MemoryPool *pool);

/**
 * Arena allocator for bulk memory management.
 */
typedef struct Arena {
    uint8_t *memory;
    size_t size;
    size_t offset;
} Arena;

/**
 * Creates a new arena allocator.
 */
Arena *arena_create(size_t size);

/**
 * Allocates memory from the arena.
 */
void *arena_alloc(Arena *arena, size_t size);

/**
 * Resets the arena to its initial state.
 */
void arena_reset(Arena *arena);

/**
 * Destroys the arena.
 */
void arena_destroy(Arena *arena);

#ifdef __cplusplus
}
#endif

#endif /* MEMORY_POOL_H */