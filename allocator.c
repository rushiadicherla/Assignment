#include <stdio.h>
#include <stdint.h>

#define POOL_SIZE (100 * 1024)

static uint8_t memory_pool[POOL_SIZE];
static uint8_t *next_free = memory_pool;

// Initialize memory pool
void init_memory() {
    next_free = memory_pool;
}

int* allocate(int size) {
    if (size <= 0) return NULL;
    if (next_free + size > memory_pool + POOL_SIZE) return NULL; // out of memory

    int *ptr = (int*)next_free;
    next_free += size; // move pointer forward
    return ptr;
}

void deallocate(int *ptr) {
    (void)ptr;
}

int main() {
    init_memory();

    int *mem[3];
    mem[0] = allocate(128);
    mem[1] = allocate(1024);
    mem[2] = allocate(4096);

    printf("mem[0]: %p\n", mem[0]);
    printf("mem[1]: %p\n", mem[1]);
    printf("mem[2]: %p\n", mem[2]);

    deallocate(mem[1]); 
    mem[1] = allocate(512); 

    printf("mem[1] after free+alloc: %p\n", mem[1]);
    return 0;
}
