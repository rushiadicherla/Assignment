#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define POOL_KB    100
#define POOL_SIZE  (POOL_KB * 1024)      
#define MIN_ALLOC  1                     
#define ALIGNMENT  (sizeof(int))         
#define TRUE  1
#define FALSE 0

  typedef struct block_header {
    size_t size;                
    int free;                    
    struct block_header *next;   
} block_header_t;

int *allocate(int size) {
    if (size <= 0) return NULL;
    if (size < MIN_ALLOC) size = MIN_ALLOC;

    size_t req_size = ALIGN_UP((size_t)size, ALIGNMENT);

    
    block_header_t *blk = find_first_fit(req_size);
    if (!blk) {
      
        coalesce_if_possible();
        blk = find_first_fit(req_size);
        if (!blk) return NULL;
    }

    
    if (blk->size >= req_size) {
        
        split_block(blk, req_size);
    } else {
      
        return NULL;
    }

    uint8_t *user_mem = (uint8_t *)blk + HEADER_SIZE;
    return (int *)user_mem;
}

void deallocate(int *ptr) {
    if (!ptr) return;

    block_header_t *hdr = ptr_to_header((void *)ptr);
    if (!hdr) {
        return;
    }

    if (hdr->free) {
        return;
    }

    hdr->free = TRUE;
    coalesce_if_possible();
}
