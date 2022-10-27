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
	struct MemBlock* blk ;
	struct MemBlock* lastBlk = NULL ;
	cprintf("\nFreeMemBlocksList:\n");
	uint8 sorted = 1 ;
	LIST_FOREACH(blk, &FreeMemBlocksList)
	{
		if (lastBlk && blk->sva < lastBlk->sva + lastBlk->size)
			sorted = 0 ;
		cprintf("[%x, %x)-->", blk->sva, blk->sva + blk->size) ;
		lastBlk = blk;
	}
	if (!sorted)	cprintf("\nFreeMemBlocksList is NOT SORTED!!\n") ;

	lastBlk = NULL ;
	cprintf("\nAllocMemBlocksList:\n");
	sorted = 1 ;
	LIST_FOREACH(blk, &AllocMemBlocksList)
	{
		if (lastBlk && blk->sva < lastBlk->sva + lastBlk->size)
			sorted = 0 ;
		cprintf("[%x, %x)-->", blk->sva, blk->sva + blk->size) ;
		lastBlk = blk;
	}
	if (!sorted)	cprintf("\nAllocMemBlocksList is NOT SORTED!!\n") ;
	cprintf("\n=========================================\n");

}

//********************************************************************************//
//********************************************************************************//

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

//===============================
// [1] INITIALIZE AVAILABLE LIST:
//===============================
void initialize_MemBlocksList(uint32 numOfBlocks)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] initialize_MemBlocksList
	// Write your code here, remove the panic and write your code
	//panic("initialize_MemBlocksList() is not implemented yet...!!");
	LIST_INIT(&AvailableMemBlocksList);
	for (int i = 0; i < MIN(numOfBlocks, MAX_MEM_BLOCK_CNT); i++) {
	    LIST_INSERT_HEAD(&AvailableMemBlocksList, &MemBlockNodes[i]);
	}
}

//===============================
// [2] FIND BLOCK:
//===============================
struct MemBlock *find_block(struct MemBlock_List *blockList, uint32 va)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] find_block
	// Write your code here, remove the panic and write your code
	panic("find_block() is not implemented yet...!!");

}

//=========================================
// [3] INSERT BLOCK IN ALLOC LIST [SORTED]:
//=========================================
void listInsert(struct MemBlock *blockToInsert)
{
	struct MemBlock *tmp = AllocMemBlocksList.lh_first;
	uint32 tmpSva = tmp->prev_next_info.le_next->sva;
	while(tmpSva < blockToInsert->sva)
	{
		tmp = tmp->prev_next_info.le_next;
		tmpSva = tmp->prev_next_info.le_next->sva;
	}
	blockToInsert->prev_next_info.le_next = tmp->prev_next_info.le_next;
	tmp->prev_next_info.le_next = blockToInsert;
	blockToInsert->prev_next_info.le_prev = tmp;
	((&(AllocMemBlocksList))->size)++ ;
}

void insert_sorted_allocList(struct MemBlock *blockToInsert)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] insert_sorted_allocList
	// Write your code here, remove the panic and write your code
	//panic("insert_sorted_allocList() is not implemented yet...!!");
	void (*insertIntoList_fn_ptr)() = listInsert;

	if(LIST_SIZE(&(AllocMemBlocksList)) == 0)
		LIST_INSERT_HEAD(&(AllocMemBlocksList), blockToInsert);
	else
	{
		if(LIST_LAST(&(AllocMemBlocksList))->sva < blockToInsert->sva)
			LIST_INSERT_TAIL(&(AllocMemBlocksList), blockToInsert);
		else if(LIST_FIRST(&(AllocMemBlocksList))->sva > blockToInsert->sva)
			LIST_INSERT_HEAD(&(AllocMemBlocksList), blockToInsert);
		else
		{
			insertIntoList_fn_ptr(blockToInsert);
		}
	}
}


//=========================================
// [4] ALLOCATE BLOCK BY FIRST FIT:
//=========================================
struct MemBlock *alloc_block_FF(uint32 size)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] alloc_block_FF
	// Write your code here, remove the panic and write your code
	//panic("alloc_block_FF() is not implemented yet...!!");
	struct MemBlock *tmp = FreeMemBlocksList.lh_first;
	//LIST_FOREACH(tmp,&(FreeMemBlocksList));
	while(tmp->prev_next_info.le_next != NULL)
	{
		if(tmp->size == size)
		{
			LIST_REMOVE(&(FreeMemBlocksList), tmp);
			return tmp;
		}
		else if(tmp->size > size)
		{
			struct MemBlock *newBlock = LIST_FIRST(&AvailableMemBlocksList);
			LIST_REMOVE(&AvailableMemBlocksList, newBlock);

			newBlock->size = size;
			newBlock->sva = tmp->sva;
			tmp->size -= size;
			tmp->sva += size ;
//			LIST_REMOVE(&(FreeMemBlocksList), tmp);
			return newBlock;
		}
		tmp = tmp->prev_next_info.le_next;
	}
	return NULL;
}

//=========================================
// [5] ALLOCATE BLOCK BY BEST FIT:
//=========================================
struct MemBlock *alloc_block_BF(uint32 size)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] alloc_block_BF
	// Write your code here, remove the panic and write your code
	panic("alloc_block_BF() is not implemented yet...!!");
}


//=========================================
// [7] ALLOCATE BLOCK BY NEXT FIT:
//=========================================
struct MemBlock *alloc_block_NF(uint32 size)
{
	//TODO: [PROJECT MS1 - BONUS] [DYNAMIC ALLOCATOR] alloc_block_NF
	// Write your code here, remove the panic and write your code
	panic("alloc_block_NF() is not implemented yet...!!");

}

//===================================================
// [8] INSERT BLOCK (SORTED WITH MERGE) IN FREE LIST:
//===================================================
void insert_sorted_with_merge_freeList(struct MemBlock *blockToInsert)
{
	//cprintf("BEFORE INSERT with MERGE: insert [%x, %x)\n=====================\n", blockToInsert->sva, blockToInsert->sva + blockToInsert->size);
	//print_mem_block_lists() ;

	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] insert_sorted_with_merge_freeList
	// Write your code here, remove the panic and write your code
	panic("insert_sorted_with_merge_freeList() is not implemented yet...!!");



	//cprintf("\nAFTER INSERT with MERGE:\n=====================\n");
	//print_mem_block_lists();

}
