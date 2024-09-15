// Nagendra Upadhyay. CS360 Lab7 Jmalloc April 9th 2023. The code below is a malloc function which is using for memory allocation and deallocation.
// The implementation uses a linked list of memory blocks, where each block has a size and a pointer to the next block in the list. 

// Including all the needed header files
#include "mymalloc.h"
#include <stdio.h>
#include <unistd.h>

// Define a block struct that holds the size of a block and a pointer to the next block
typedef struct block_allo {
    int size;
    struct block_allo* next;
} block_allo;

// Define a pointer to the beginning of the memory block
block_allo* begin_p = NULL;

// Allocate a large block of memory using sbrk
block_allo* allocate_large_block(size_t size) {
    block_allo* mem_allo = sbrk(size);
    mem_allo->size = size;
    return mem_allo;
}

// Allocate a small block of memory using sbrk
block_allo* allocate_small_block(size_t size) {
    // Allocate a block of size to be extactly 8192
    block_allo* mem_allo = sbrk(8192);
    // Calculate the size of the remaining memory and store it in the size field of the block
    mem_allo->size = 8192 - size;
    // Free the first 8 bytes of the block
    my_free((void*)mem_allo + 8);

    // Calculate the address of the memory block and it needs to be returned and store its size
    block_allo* bloc_mem = (void*)mem_allo + mem_allo->size;
    bloc_mem->size = size;
    return bloc_mem;
}
 
// Allocate a block of memory of size 'size'
void* allocate_block(size_t size) {
    if (size > 8192) {
        // If the requested size is greater than 8192, allocate a large block
        return (void*)allocate_large_block(size) + 8;
    } else {
        // Or, allocate a small block
        return (void*)allocate_small_block(size) + 8;
    }
}
 
// Allocate a block of memory of size 'size' with 8 byte alignment
void* my_malloc(size_t size) {
    // Calculate the alignment required for the block
    int alignments = 8 - (size % 8);
    if (alignments == 8) {
        alignments = 0;
    }

    // Calculate the total size of the block, including space for alignment and bookkeeping information
    size_t bytes = alignments + size + 8;
    // Traverse the free list to find a block that is large enough to hold the requested block
    block_allo* block_p = begin_p;
    while (block_p && block_p->size < bytes) {
        block_p = block_p->next;
    }  
      
    // If a block was found, check if it can be split
    if (block_p != NULL) {
        if ((block_p->size - bytes) >= 8) {
            if (block_p->size == bytes) {
                // If the block is exactly the same size as the requested block, return it
                return (void*)block_p + 8;
            } else {
                // Otherwise, split the block and return the second part
                block_allo* tmp = (void*)block_p + block_p->size - bytes;
                block_p->size = block_p->size - bytes;
                tmp->size = bytes;
                return (void*)tmp + 8;
            } 
        } else if (block_p == begin_p) {
            // If the block is at the beginning of the free list, remove block_p from the list and return block_p
            begin_p = begin_p->next;
            return (void*)block_p + 8;
        } else {
            // Otherwise, remove the block from the free list and return block_p
            return (void*)block_p + 8;
        }
    } else {
        // If no block was found, allocate a new block with the requested size
        return allocate_block(bytes);
    }
}
  
// This function is used to free the memory block pointed to by ptr
void my_free(void* ptr) {
    // Pointer to the beginning of the free list
    block_allo* p = begin_p;
    // Decrement the pointer to point to the beginning of the allocation block
    ptr = ptr - 8;
    
    // If the free list is empty, add the memory block to the beginning of the list
    if (!begin_p) {
        begin_p = (block_allo*)ptr;
        begin_p->next = NULL;
        return;
    }
    
    // If the memory block is before the beginning of the free list, add it to the beginning of the list
    if ((char*)ptr < (char*)begin_p) {
        ((block_allo*)ptr)->next = begin_p;
        begin_p = (block_allo*)ptr;
        return;
    }
 
    // Otherwise, iterate through the free list to find the correct position to insert the memory block
    while (p->next && (char*)p->next < (char*)ptr) {
        p = p->next;
    }

    // Insert the memory block into the free list
    ((block_allo*)ptr)->next = p->next;
    p->next = (block_allo*)ptr;
}

// This function returns the beginning of the free list
void* free_list_begin() { return begin_p; }
// This function returns the next node in the free list
void* free_list_next(void* nodes) { return ((block_allo*)nodes)->next; }

// This function coalesces adjacent blocks in the free list
void coalesce_free_list() {
    // Pointer to the current node in the free list
    block_allo** node_ptr = &begin_p;
    // Iterate through the free list
    while (*node_ptr) {
        // If the current node and the next node in the list are adjacent, coalesce them
        if ((char*)*node_ptr + (*node_ptr)->size == (char*)(*node_ptr)->next) {
            (*node_ptr)->size += (*node_ptr)->next->size;
            (*node_ptr)->next = (*node_ptr)->next->next;
        } else {
            // Otherwise, move on to the next node in the list
            node_ptr = &(*node_ptr)->next;
        }
    }
}
