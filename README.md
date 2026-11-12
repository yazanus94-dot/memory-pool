# Memory Pool

A high-performance memory allocation library with O(1) allocation and deallocation using free lists and arena allocators.

![C](https://img.shields.io/badge/C-11-lightgrey)
![License](https://img.shields.io/badge/License-MIT-green)

## Overview

Memory Pool provides fixed-size memory pool allocation with O(1) allocation and deallocation. Unlike the standard `malloc`/`free`, which may traverse free lists or split blocks, a memory pool pre-allocates a contiguous block and uses a free list for instant reuse. This makes it ideal for real-time systems, game engines, and high-frequency trading where allocation latency must be predictable.

## Key Features

- **O(1) Allocation**: Constant-time allocation via free list
- **O(1) Deallocation**: Constant-time free via free list push
- **Arena Allocator**: Bulk memory management with O(1) reset
- **Zero Fragmentation**: Fixed-size blocks eliminate external fragmentation
- **Cache-Friendly**: Contiguous memory improves cache locality
- **Error Handling**: NULL checks on all operations

## Architecture

```
┌─────────────────────────────────────────┐
│            MemoryPool                    │
│  ┌──────────┬──────────┬──────────┐    │
│  │ Block 0  │ Block 1  │ Block 2  │    │
│  │ (free)   │ (free)   │ (free)   │    │
│  └──────────┴──────────┴──────────┘    │
│                    ↑                     │
│               free_list                 │
└─────────────────────────────────────────┘
```

## Installation

```bash
git clone https://github.com/yourusername/memory-pool
cd memory-pool
make
```

## Quick Start

```c
#include "memory_pool.h"
#include <stdio.h>

int main() {
    // Create a pool of 100 blocks, each 64 bytes
    MemoryPool *pool = memory_pool_create(64, 100);
    
    // Allocate
    void *ptr = memory_pool_alloc(pool);
    
    // Use the memory
    ((char*)ptr)[0] = 'A';
    
    // Free back to pool
    memory_pool_free(pool, ptr);
    
    // Cleanup
    memory_pool_destroy(pool);
    return 0;
}
```

## Testing

```bash
make test
```

## License

MIT