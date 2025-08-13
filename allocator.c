#include <stdio.h>
#include <stdint.h>

#define POOL_SIZE (100 * 1024)
static uint8_t memory_pool[POOL_SIZE];
typedef struct Block {
    int size;               
    int free;              
    struct Block *next;    
} Block;

static Block *free_list = NULL;

void init_memory() {
    free_list = (Block*)memory_pool;
    free_list->size = POOL_SIZE - sizeof(Block);
    free_list->free = 1;
    free_list->next = NULL;
}

int* allocate(int size) {
    if (size <= 0) return NULL;

    Block *curr = free_list;
    while (curr) {
        if (curr->free && curr->size >= size) {
      
            if (curr->size > size + sizeof(Block)) {
                Block *new_block = (Block*)((uint8_t*)curr + sizeof(Block) + size);
                new_block->size = curr->size - size - sizeof(Block);
                new_block->free = 1;
                new_block->next = curr->next;
                curr->next = new_block;
                curr->size = size;
            }
            curr->free = 0;
            return (int*)((uint8_t*)curr + sizeof(Block));
        }
        curr = curr->next;
    }
    return NULL; 
}

void deallocate(int *ptr) {
    if (!ptr) return;
    Block *block = (Block*)((uint8_t*)ptr - sizeof(Block));
    block->free = 1;

    if (block->next && block->next->free) {
        block->size += sizeof(Block) + block->next->size;
        block->next = block->next->next;
    }
}

int main() {
    init_memory();

    int *mem[100];
    mem[0] = allocate(128);
    mem[1] = allocate(1024);

    deallocate(mem[1]);
    mem[1] = allocate(512);

    printf("mem[0]: %p\n", mem[0]);
    printf("mem[1]: %p\n", mem[1]);

    return 0;
}
