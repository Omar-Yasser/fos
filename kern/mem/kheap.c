#include "kheap.h"
#include <inc/memlayout.h>
#include <inc/dynamic_allocator.h>
#include "memory_manager.h"
#define MAX_NUM_OF_FRAMES (1 << 20) + 5
#define NEURTAL_ELEM 0
//==================================================================//
//==================================================================//
// NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)//
//==================================================================//
//==================================================================//
uint32 PA[MAX_NUM_OF_FRAMES];
void initialize_dyn_block_system()
{
    // TODO: [PROJECT MS2] [KERNEL HEAP] initialize_dyn_block_system
    //  your code is here, remove the panic and write your code
    //  kpanic_into_prompt("initialize_dyn_block_system() is not implemented yet...!!");

    //[1] Initialize two lists (AllocMemBlocksList & FreeMemBlocksList) [Hint: use LIST_INIT()]

    LIST_INIT(&AllocMemBlocksList);
    LIST_INIT(&FreeMemBlocksList);
#if STATIC_MEMBLOCK_ALLOC
    // DO NOTHING
#else
    /*[2] Dynamically allocate the array of MemBlockNodes
     * 	remember to:
     * 		1. set MAX_MEM_BLOCK_CNT with the chosen size of the array
     * 		2. allocation should be aligned on PAGE boundary
     * 	HINT: can use alloc_chunk(...) function
     */
    MAX_MEM_BLOCK_CNT = (uint32)(KERNEL_HEAP_MAX - KERNEL_HEAP_START) / PAGE_SIZE;
    MemBlockNodes = (struct MemBlock *)KERNEL_HEAP_START;
    allocate_chunk(ptr_page_directory, KERNEL_HEAP_START, (sizeof(struct MemBlock)) * MAX_MEM_BLOCK_CNT, PERM_WRITEABLE);
#endif
    //[3] Initialize AvailableMemBlocksList by filling it with the MemBlockNodes
    initialize_MemBlocksList(MAX_MEM_BLOCK_CNT);
    //[4] Insert a new MemBlock with the remaining heap size into the FreeMemBlocksList
    struct MemBlock *newBlock = LIST_FIRST(&AvailableMemBlocksList);
    LIST_REMOVE(&AvailableMemBlocksList, newBlock);

    newBlock->size = ((KERNEL_HEAP_MAX - KERNEL_HEAP_START) - ROUNDUP(MAX_MEM_BLOCK_CNT * sizeof(struct MemBlock), PAGE_SIZE));
    newBlock->sva = (KERNEL_HEAP_START + ROUNDUP(MAX_MEM_BLOCK_CNT * sizeof(struct MemBlock), PAGE_SIZE));

    LIST_INSERT_HEAD(&(FreeMemBlocksList), newBlock);
    memset(PA, NEURTAL_ELEM, sizeof(PA));
}

void *kmalloc(unsigned int size)
{
    // TODO: [PROJECT MS2] [KERNEL HEAP] kmalloc
    //  your code is here, remove the panic and write your code
    // kpanic_into_prompt("kmalloc() is not implemented yet...!!");

    // NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)
    // refer to the project presentation and documentation for details
    //  use "isKHeapPlacementStrategyFIRSTFIT() ..." functions to check the current strategy
    size = ROUNDUP(size, PAGE_SIZE);
    struct MemBlock *blk;
    if (isKHeapPlacementStrategyBESTFIT())
        blk = alloc_block_BF(size);
    else if (isKHeapPlacementStrategyFIRSTFIT())
        blk = alloc_block_FF(size);
    else
        blk = alloc_block_NF(size);
    if (blk == NULL)
        return NULL;
    int ret = allocate_chunk(ptr_page_directory, blk->sva, size, PERM_WRITEABLE);
    if (ret == 0)
    {
        uint32 start_source_va = ROUNDDOWN(blk->sva, PAGE_SIZE), end_source_va = ROUNDUP(blk->sva + size, PAGE_SIZE);
        while (start_source_va < end_source_va)
        {
            PA[virtual_to_physical(ptr_page_directory, start_source_va) >> 12] = start_source_va;
            start_source_va += PAGE_SIZE;
        }
        insert_sorted_allocList(blk);
        return (void *)blk->sva;
    }

    // change this "return" according to your answer
    return NULL;
}

void kfree(void *virtual_address)
{
    // TODO: [PROJECT MS2] [KERNEL HEAP] kfree
    //  Write your code here, remove the panic and write your code
    // panic("kfree() is not implemented yet...!!");
    struct MemBlock *blk;
    blk = find_block(&AllocMemBlocksList, (uint32)virtual_address);
    if (blk == NULL)
        return;
    LIST_REMOVE(&AllocMemBlocksList, blk);
    uint32 sva = blk->sva, eva = sva + blk->size;
    sva = ROUNDDOWN(sva, PAGE_SIZE), eva = ROUNDUP(eva, PAGE_SIZE);
    while (sva < eva)
    {
        PA[virtual_to_physical(ptr_page_directory, sva) >> 12] = NEURTAL_ELEM;
        unmap_frame(ptr_page_directory, sva);
        sva += PAGE_SIZE;
    }
    insert_sorted_with_merge_freeList(blk);
}

unsigned int kheap_virtual_address(unsigned int physical_address)
{
    // TODO: [PROJECT MS2] [KERNEL HEAP] kheap_virtual_address
    //  Write your code here, remove the panic and write your code
    // panic("kheap_virtual_address() is not implemented yet...!!");

    return PA[physical_address >> 12];
    // return the virtual address corresponding to given physical_address
    // refer to the project presentation and documentation for details
    // EFFICIENT IMPLEMENTATION ~O(1) IS REQUIRED ==================
}

unsigned int kheap_physical_address(unsigned int virtual_address)
{
    // TODO: [PROJECT MS2] [KERNEL HEAP] kheap_physical_address
    //  Write your code here, remove the panic and write your code
    // panic("kheap_physical_address() is not implemented yet...!!");

    // return the physical address corresponding to given virtual_address
    // refer to the project presentation and documentation for details
    return virtual_to_physical(ptr_page_directory, virtual_address);
}

void kfreeall()
{
    panic("Not implemented!");
}

void kshrink(uint32 newSize)
{
    panic("Not implemented!");
}

void kexpand(uint32 newSize)
{
    panic("Not implemented!");
}

//=================================================================================//
//============================== BONUS FUNCTION ===================================//
//=================================================================================//
// krealloc():

//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to kmalloc().
//	A call with new_size = zero is equivalent to kfree().

void *krealloc(void *virtual_address, uint32 new_size)
{
    // TODO: [PROJECT MS2 - BONUS] [KERNEL HEAP] krealloc
    //  Write your code here, remove the panic and write your code
    // panic("krealloc() is not implemented yet...!!");
    if (virtual_address == NULL)
        return kmalloc(new_size);
    if (new_size == 0)
    {
        kfree(virtual_address);
        return virtual_address;
    }
    struct MemBlock *blk;
    blk = find_block(&AllocMemBlocksList, (uint32)virtual_address);
    if (blk == NULL)
        return kmalloc(new_size);
    if (blk->size >= new_size)
        return virtual_address;
    int ret = allocate_chunk(ptr_page_directory, blk->sva + blk->size, new_size - blk->size, PERM_WRITEABLE);
    if (ret == 0)
    {
        blk->size += ROUNDUP(new_size - blk->size, PAGE_SIZE);
        return virtual_address;
    }
    void *ptr = kmalloc(new_size);
    copy_paste_chunk(ptr_page_directory, (uint32)virtual_address, (uint32)ptr, blk->size);
    kfree(virtual_address);
    return ptr;
}
