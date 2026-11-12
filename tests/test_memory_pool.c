/**
 * Memory Pool - Test Suite
 */

#include "../include/memory_pool.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

static int tests_run = 0;
static int tests_passed = 0;

#define TEST(name) do { \
    tests_run++; \
    printf("  TEST: %s ... ", name); \
} while(0)

#define PASS() do { \
    tests_passed++; \
    printf("PASS\n"); \
} while(0)

#define FAIL(msg) do { \
    printf("FAIL: %s\n", msg); \
} while(0)

void test_create_destroy() {
    TEST("create and destroy");
    MemoryPool *pool = memory_pool_create(64, 100);
    assert(pool != NULL);
    assert(memory_pool_capacity(pool) == 100);
    assert(memory_pool_free_count(pool) == 100);
    memory_pool_destroy(pool);
    PASS();
}

void test_alloc_free() {
    TEST("alloc and free");
    MemoryPool *pool = memory_pool_create(32, 10);
    void *ptr1 = memory_pool_alloc(pool);
    void *ptr2 = memory_pool_alloc(pool);
    assert(ptr1 != NULL);
    assert(ptr2 != NULL);
    assert(ptr1 != ptr2);
    assert(memory_pool_free_count(pool) == 8);
    memory_pool_free(pool, ptr1);
    memory_pool_free(pool, ptr2);
    assert(memory_pool_free_count(pool) == 10);
    memory_pool_destroy(pool);
    PASS();
}

void test_alloc_exhaustion() {
    TEST("alloc exhaustion returns NULL");
    MemoryPool *pool = memory_pool_create(16, 4);
    void *p1 = memory_pool_alloc(pool);
    void *p2 = memory_pool_alloc(pool);
    void *p3 = memory_pool_alloc(pool);
    void *p4 = memory_pool_alloc(pool);
    void *p5 = memory_pool_alloc(pool);
    assert(p1 != NULL);
    assert(p2 != NULL);
    assert(p3 != NULL);
    assert(p4 != NULL);
    assert(p5 == NULL);
    memory_pool_destroy(pool);
    PASS();
}

void test_free_list_reuse() {
    TEST("free list reuse");
    MemoryPool *pool = memory_pool_create(16, 3);
    void *p1 = memory_pool_alloc(pool);
    void *p2 = memory_pool_alloc(pool);
    memory_pool_free(pool, p1);
    void *p3 = memory_pool_alloc(pool);
    assert(p3 == p1);  // Should reuse the freed block
    memory_pool_destroy(pool);
    PASS();
}

void test_invalid_params() {
    TEST("invalid parameters return NULL");
    assert(memory_pool_create(0, 10) == NULL);
    assert(memory_pool_create(10, 0) == NULL);
    assert(memory_pool_alloc(NULL) == NULL);
    memory_pool_free(NULL, NULL);  // Should not crash
    PASS();
}

void test_arena() {
    TEST("arena allocator");
    Arena *arena = arena_create(1024);
    assert(arena != NULL);
    void *p1 = arena_alloc(arena, 64);
    void *p2 = arena_alloc(arena, 128);
    assert(p1 != NULL);
    assert(p2 != NULL);
    assert((char*)p2 - (char*)p1 >= 64);
    arena_reset(arena);
    void *p3 = arena_alloc(arena, 64);
    assert(p3 == p1);  // After reset, should reuse memory
    arena_destroy(arena);
    PASS();
}

void test_small_block_size() {
    TEST("block size smaller than pointer");
    MemoryPool *pool = memory_pool_create(4, 10);
    assert(pool != NULL);
    void *ptr = memory_pool_alloc(pool);
    assert(ptr != NULL);
    memory_pool_destroy(pool);
    PASS();
}

int main() {
    printf("Memory Pool Test Suite\n");
    printf("======================\n\n");
    
    test_create_destroy();
    test_alloc_free();
    test_alloc_exhaustion();
    test_free_list_reuse();
    test_invalid_params();
    test_arena();
    test_small_block_size();
    
    printf("\nResults: %d/%d tests passed\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}