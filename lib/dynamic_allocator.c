/*
 * dyn_block_management.c
 *
 *  Created on: Sep 21, 2022
 *      Author: HP
 */
#include <inc/assert.h>
#include <inc/string.h>
#include "../inc/dynamic_allocator.h"

//==================================================================================//
//============================== GIVEN FUNCTIONS ===================================//
//==================================================================================//

//===========================
// PRINT MEM BLOCK LISTS:
//===========================

void print_mem_block_lists()
{
    cprintf("\n=========================================\n");
    struct MemBlock *blk;
    struct MemBlock *lastBlk = NULL;
    cprintf("\nFreeMemBlocksList:\n");
    uint8 sorted = 1;
    LIST_FOREACH(blk, &FreeMemBlocksList)
    {
        if (lastBlk && blk->sva < lastBlk->sva + lastBlk->size)
            sorted = 0;
        cprintf("[%x, %x)-->", blk->sva, blk->sva + blk->size);
        lastBlk = blk;
    }
    if (!sorted)
        cprintf("\nFreeMemBlocksList is NOT SORTED!!\n");

    lastBlk = NULL;
    cprintf("\nAllocMemBlocksList:\n");
    sorted = 1;
    LIST_FOREACH(blk, &AllocMemBlocksList)
    {
        if (lastBlk && blk->sva < lastBlk->sva + lastBlk->size)
            sorted = 0;
        cprintf("[%x, %x)-->", blk->sva, blk->sva + blk->size);
        lastBlk = blk;
    }
    if (!sorted)
        cprintf("\nAllocMemBlocksList is NOT SORTED!!\n");
    cprintf("\n=========================================\n");
}

//********************************************************************************//
//********************************************************************************//

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

struct MemBlock *lastAllocBlk = NULL;

//===============================
// [1] INITIALIZE AVAILABLE LIST:
//===============================
void initialize_MemBlocksList(uint32 numOfBlocks)
{
    // TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] initialize_MemBlocksList
    //  Write your code here, remove the panic and write your code
    //  panic("initialize_MemBlocksList() is not implemented yet...!!");
    LIST_INIT(&AvailableMemBlocksList);
    for (int i = 0; i < MIN(numOfBlocks, MAX_MEM_BLOCK_CNT); i++)
    {
        LIST_INSERT_HEAD(&AvailableMemBlocksList, &MemBlockNodes[i]);
    }
}

//===============================
// [2] FIND BLOCK:
//===============================
struct MemBlock *find_block(struct MemBlock_List *blockList, uint32 va)
{
    // TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] find_block
    //  Write your code here, remove the panic and write your code
    //  panic("find_block() is not implemented yet...!!");
    struct MemBlock *blk;
    LIST_FOREACH(blk, blockList)
    {
        if (blk->sva == va)
        {
            return blk;
        }
    }
    return NULL;
}

//=========================================
// [3] INSERT BLOCK IN ALLOC LIST [SORTED]:
//=========================================
void insert_block(struct MemBlock *blockToInsert, struct MemBlock_List *blockList)
{
    if (LIST_FIRST(blockList) == NULL || LIST_FIRST(blockList)->sva > blockToInsert->sva)
        LIST_INSERT_HEAD(blockList, blockToInsert);
    else if (LIST_LAST(blockList)->sva < blockToInsert->sva)
        LIST_INSERT_TAIL(blockList, blockToInsert);
    else
    {
        struct MemBlock *blk = LIST_FIRST(blockList);
        while (blk != NULL && blk->sva < blockToInsert->sva)
        {
            blk = blk->prev_next_info.le_next;
        }
        blockToInsert->prev_next_info.le_next = blk;
        blockToInsert->prev_next_info.le_prev = blk->prev_next_info.le_prev;
        blk->prev_next_info.le_prev->prev_next_info.le_next = blockToInsert;
        blk->prev_next_info.le_prev = blockToInsert;
        blockList->size++;
    }
}

void insert_sorted_allocList(struct MemBlock *blockToInsert)
{
    // TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] insert_sorted_allocList
    //  Write your code here, remove the panic and write your code
    // panic("insert_sorted_allocList() is not implemented yet...!!");

    insert_block(blockToInsert, &AllocMemBlocksList);
}

//=========================================
// [4] ALLOCATE BLOCK BY FIRST FIT:
//=========================================
struct MemBlock *alloc_block_FF(uint32 size)
{
    // TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] alloc_block_FF
    //  Write your code here, remove the panic and write your code
    // panic("alloc_block_FF() is not implemented yet...!!");
    struct MemBlock *blk = FreeMemBlocksList.lh_first;
    // LIST_FOREACH(blk,&(FreeMemBlocksList));
    while (blk->prev_next_info.le_next != NULL)
    {
        if (blk->size == size)
        {
            LIST_REMOVE(&(FreeMemBlocksList), blk);
            return lastAllocBlk = blk;
        }
        else if (blk->size > size)
        {
            struct MemBlock *newBlock = LIST_FIRST(&AvailableMemBlocksList);
            LIST_REMOVE(&AvailableMemBlocksList, newBlock);

            newBlock->size = size;
            newBlock->sva = blk->sva;
            blk->size -= size;
            blk->sva += size;
            return lastAllocBlk = newBlock;
        }
        blk = blk->prev_next_info.le_next;
    }
    return NULL;
}

//=========================================
// [5] ALLOCATE BLOCK BY BEST FIT:
//=========================================
struct MemBlock *alloc_block_BF(uint32 size)
{
    // TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] alloc_block_BF
    //  Write your code here, remove the panic and write your code
    // panic("alloc_block_BF() is not implemented yet...!!");
    struct MemBlock *blk, *BF;
    uint32 maxi = __UINT32_MAX__;
    LIST_FOREACH(blk, &FreeMemBlocksList)
    {
        if (blk->size >= size && blk->size < maxi)
        {
            BF = blk;
            maxi = blk->size;
        }
    }
    if (maxi == __UINT32_MAX__)
        return NULL;
    if (BF->size == size)
    {
        LIST_REMOVE(&FreeMemBlocksList, BF);
        return lastAllocBlk = BF;
    }
    struct MemBlock *newBlock = LIST_FIRST(&AvailableMemBlocksList);
    LIST_REMOVE(&AvailableMemBlocksList, newBlock);
    newBlock->size = size;
    newBlock->sva = BF->sva;
    BF->size -= size;
    BF->sva += size;
    return lastAllocBlk = newBlock;
}

//=========================================
// [7] ALLOCATE BLOCK BY NEXT FIT:
//=========================================

struct MemBlock *find_block_NF(uint32 size, uint32 sva)
{
    struct MemBlock *blk;
    LIST_FOREACH(blk, &FreeMemBlocksList)
    {
        if (blk->sva < sva)
            continue;
        if (blk->size == size)
        {
            LIST_REMOVE(&FreeMemBlocksList, blk);
            return lastAllocBlk = blk;
        }
        if (blk->size > size)
        {
            struct MemBlock *newBlock = LIST_FIRST(&AvailableMemBlocksList);
            LIST_REMOVE(&AvailableMemBlocksList, newBlock);

            newBlock->size = size;
            newBlock->sva = blk->sva;
            blk->size -= size;
            blk->sva += size;
            return lastAllocBlk = newBlock;
        }
    }
    return NULL;
}
struct MemBlock *alloc_block_NF(uint32 size)
{
    // TODO: [PROJECT MS1 - BONUS] [DYNAMIC ALLOCATOR] alloc_block_NF
    //  Write your code here, remove the panic and write your code
    // panic("alloc_block_NF() is not implemented yet...!!");

    struct MemBlock *block_NF = find_block_NF(size, (lastAllocBlk == NULL ? 0 : lastAllocBlk->sva));
    if (block_NF == NULL)
        block_NF = find_block_NF(size, 0);
    return block_NF;
}

//===================================================
// [8] INSERT BLOCK (SORTED WITH MERGE) IN FREE LIST:
//===================================================

void merge_freeList(struct MemBlock *l, struct MemBlock *r)
{   
    if (l == NULL || r == NULL)
        return;
    if (l->sva + l->size == r->sva)
    {
        l->size += r->size;
        r->size = 0;
        r->sva = 0;
        LIST_REMOVE(&FreeMemBlocksList, r);
        LIST_INSERT_HEAD(&AvailableMemBlocksList, r);
    }
}
void insert_sorted_with_merge_freeList(struct MemBlock *blockToInsert)
{
    // cprintf("BEFORE INSERT with MERGE: insert [%x, %x)\n=====================\n", blockToInsert->sva, blockToInsert->sva + blockToInsert->size);
    // print_mem_block_lists() ;

    // TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] insert_sorted_with_merge_freeList
    //  Write your code here, remove the panic and write your code
    // panic("insert_sorted_with_merge_freeList() is not implemented yet...!!");

    insert_block(blockToInsert, &FreeMemBlocksList);
    merge_freeList(blockToInsert, blockToInsert->prev_next_info.le_next);
    merge_freeList(blockToInsert->prev_next_info.le_prev, blockToInsert);

    // cprintf("\nAFTER INSERT with MERGE:\n=====================\n");
    // print_mem_block_lists();
}
