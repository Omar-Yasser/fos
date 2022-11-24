/*
 * chunk_operations.c
 *
 *  Created on: Oct 12, 2022
 *      Author: HP
 */

#include <kern/trap/fault_handler.h>
#include <kern/disk/pagefile_manager.h>
#include "kheap.h"
#include "memory_manager.h"

/******************************/
/*[1] RAM CHUNKS MANIPULATION */
/******************************/

//===============================
// 1) CUT-PASTE PAGES IN RAM:
//===============================
// This function should cut-paste the given number of pages from source_va to dest_va
// if the page table at any destination page in the range is not exist, it should create it
// Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
void cut_page(uint32 *page_directory, uint32 source_va, uint32 dest_va, uint32 perms)
{
	uint32 *ptr_page_table_source = NULL;
	uint32 *ptr_page_table_dest = NULL;
	get_page_table(page_directory, source_va, &ptr_page_table_source);
	if (ptr_page_table_source != NULL)
	{
		get_page_table(page_directory, dest_va, &ptr_page_table_dest);
		if (ptr_page_table_dest == NULL)
		{
			ptr_page_table_dest = create_page_table(page_directory, dest_va);
		}
		struct FrameInfo *ptr_frame_info_source = get_frame_info(page_directory, source_va, &ptr_page_table_source);
		map_frame(page_directory, ptr_frame_info_source, dest_va, perms);
		unmap_frame(page_directory, source_va);
	}
	return;
}
int cut_paste_pages(uint32 *page_directory, uint32 source_va, uint32 dest_va, uint32 num_of_pages)
{
	// TODO: [PROJECT MS2] [CHUNK OPERATIONS] cut_paste_pages
	//  Write your code here, remove the panic and write your code
	// panic("cut_paste_pages() is not implemented yet...!!");
	uint32 size = num_of_pages * PAGE_SIZE;
	uint32 start_source_va = ROUNDDOWN(source_va, PAGE_SIZE), end_source_va = ROUNDUP(source_va + size, PAGE_SIZE);
	uint32 start_dest_va = ROUNDDOWN(dest_va, PAGE_SIZE), end_dest_va = ROUNDUP(dest_va + size, PAGE_SIZE);

	// check if any destination page exists first before sharing
	// uint32 ptr_page_table_dest = NULL;
	source_va = start_source_va;
	dest_va = start_dest_va;

	while (dest_va < end_dest_va)
	{
		uint32 *ptr_page_table_dest = NULL;
		struct FrameInfo *ptr_frame_info_dest = get_frame_info(page_directory, dest_va, &ptr_page_table_dest);
		if (ptr_frame_info_dest != NULL)
		{
			return -1;
		}
		dest_va += PAGE_SIZE;
	}
	dest_va = start_dest_va;
	// begin cutting page by page
	while (source_va < end_source_va)
	{
		cut_page(page_directory, source_va, dest_va, pt_get_page_permissions(page_directory, source_va));
		source_va += PAGE_SIZE;
		dest_va += PAGE_SIZE;
	}
	return 0;
}

//===============================
// 2) COPY-PASTE RANGE IN RAM:
//===============================
// This function should copy-paste the given size from source_va to dest_va
// if the page table at any destination page in the range is not exist, it should create it
// Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int copy_paste_chunk(uint32 *page_directory, uint32 source_va, uint32 dest_va, uint32 size)
{
	// TODO: [PROJECT MS2] [CHUNK OPERATIONS] copy_paste_chunk
	//  Write your code here, remove the panic and write your code
	// panic("copy_paste_chunk() is not implemented yet...!!");

	uint32 start_dest_va = ROUNDDOWN(dest_va, PAGE_SIZE), end_dest_va = ROUNDUP(dest_va + size, PAGE_SIZE),
		   start_src_va = ROUNDDOWN(source_va, PAGE_SIZE), end_src_va = ROUNDUP(source_va + size, PAGE_SIZE), va = start_dest_va;

	// check if any destination page has read-only permissions first before copying
	while (start_dest_va < end_dest_va)
	{
		uint32 *ptr_page_table_dest = NULL;
		struct FrameInfo *ptr_frame_info_va = get_frame_info(page_directory, start_dest_va, &ptr_page_table_dest);
		if (ptr_frame_info_va != NULL && (ptr_page_table_dest[PTX(start_dest_va)] & PERM_WRITEABLE) == 0)
		{
			return -1;
		}
		start_dest_va += PAGE_SIZE;
	}

	start_dest_va = va;
	while (start_dest_va < end_dest_va && start_src_va < end_src_va)
	{
		uint32 *ptr_page_table_va = NULL;
		struct FrameInfo *ptr_frame_info_va = get_frame_info(page_directory, start_dest_va, &ptr_page_table_va);
		if (ptr_frame_info_va == NULL)
		{
			allocate_frame(&ptr_frame_info_va);
			map_frame(page_directory, ptr_frame_info_va, start_dest_va, (pt_get_page_permissions(page_directory, start_src_va) & PERM_USER) | PERM_WRITEABLE);
		}
		start_dest_va += PAGE_SIZE;
		start_src_va += PAGE_SIZE;
	}

	while (size-- > 0)
	{
		unsigned char *source = (unsigned char *)source_va;
		unsigned char *dest = (unsigned char *)dest_va;
		*dest = *source;
		source_va++;
		dest_va++;
	}
	return 0;
}

//===============================
// 3) SHARE RANGE IN RAM:
//===============================
// This function should share the given size from dest_va with the source_va
// Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
void share_page(uint32 *page_directory, uint32 source_va, uint32 dest_va, uint32 perms)
{
	uint32 *ptr_page_table_source = NULL;
	uint32 *ptr_page_table_dest = NULL;
	get_page_table(page_directory, source_va, &ptr_page_table_source);
	if (ptr_page_table_source != NULL)
	{
		get_page_table(page_directory, dest_va, &ptr_page_table_dest);
		if (ptr_page_table_dest == NULL)
		{
			ptr_page_table_dest = create_page_table(page_directory, dest_va);
		}
		struct FrameInfo *ptr_frame_info_source = get_frame_info(page_directory, source_va, &ptr_page_table_source);
		map_frame(page_directory, ptr_frame_info_source, dest_va, perms);
	}
	return;
}
int share_chunk(uint32 *page_directory, uint32 source_va, uint32 dest_va, uint32 size, uint32 perms)
{
	// TODO: [PROJECT MS2] [CHUNK OPERATIONS] share_chunk
	//  Write your code here, remove the panic and write your code
	//  panic("share_chunk() is not implemented yet...!!");
	uint32 start_source_va = ROUNDDOWN(source_va, PAGE_SIZE), end_source_va = ROUNDUP(source_va + size, PAGE_SIZE);
	uint32 start_dest_va = ROUNDDOWN(dest_va, PAGE_SIZE), end_dest_va = ROUNDUP(dest_va + size, PAGE_SIZE);

	// check if any destination page exists first before sharing
	// uint32 ptr_page_table_dest = NULL;
	source_va = start_source_va;
	dest_va = start_dest_va;

	while (dest_va < end_dest_va)
	{
		uint32 *ptr_page_table_dest = NULL;
		struct FrameInfo *ptr_frame_info_dest = get_frame_info(page_directory, dest_va, &ptr_page_table_dest);
		if (ptr_frame_info_dest != NULL)
		{
			return -1;
		}
		dest_va += PAGE_SIZE;
	}
	dest_va = start_dest_va;
	// begin sharing page by page
	while (source_va < end_source_va)
	{
		share_page(page_directory, source_va, dest_va, perms);
		source_va += PAGE_SIZE;
		dest_va += PAGE_SIZE;
	}
	return 0;
}

//===============================
// 4) ALLOCATE CHUNK IN RAM:
//===============================
// This function should allocate in RAM the given range [va, va+size)
// Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int allocate_chunk(uint32 *page_directory, uint32 va, uint32 size, uint32 perms)
{
	// TODO: [PROJECT MS2] [CHUNK OPERATIONS] allocate_chunk
	//  Write your code here, remove the panic and write your code
	// panic("allocate_chunk() is not implemented yet...!!");
	// panic("%d\n", calculate_available_frames);

	uint32 start_source_va = ROUNDDOWN(va, PAGE_SIZE), end_source_va = ROUNDUP(va + size, PAGE_SIZE);
	while (start_source_va < end_source_va)
	{
		uint32 *ptr_page_table_source = NULL;
		struct FrameInfo *ptr_frame_info_source = get_frame_info(page_directory, start_source_va, &ptr_page_table_source);

		if (ptr_frame_info_source != NULL)
		{
			return -1;
		}
		start_source_va += PAGE_SIZE;
	}
	start_source_va = ROUNDDOWN(va, PAGE_SIZE);
	while (start_source_va < end_source_va)
	{
		uint32 *ptr_page_table_source = NULL;
		struct FrameInfo *ptr_frame_info_source = get_frame_info(page_directory, start_source_va, &ptr_page_table_source);
		allocate_frame(&ptr_frame_info_source);
		map_frame(page_directory, ptr_frame_info_source, start_source_va, perms);
		start_source_va += PAGE_SIZE;
	}
	return 0;
}

/*BONUS*/
//=====================================
// 5) CALCULATE ALLOCATED SPACE IN RAM:
//=====================================
void calculate_allocated_space(uint32 *page_directory, uint32 sva, uint32 eva, uint32 *num_tables, uint32 *num_pages)
{
	// TODO: [PROJECT MS2 - BONUS] [CHUNK OPERATIONS] calculate_allocated_space
	//  Write your code here, remove the panic and write your code
	//  panic("calculate_allocated_space() is not implemented yet...!!");
	uint32 *ptr_last_located_page_table = NULL;
	sva = ROUNDDOWN(sva, PAGE_SIZE), eva = ROUNDUP(eva, PAGE_SIZE), *num_tables = 0, *num_pages = 0;
	while (sva < eva)
	{
		uint32 *ptr_page_table_source = NULL;
		int ret = get_page_table(page_directory, sva, &ptr_page_table_source);
		if (ret == TABLE_IN_MEMORY)
		{
			if ((ptr_page_table_source[PTX(sva)] & PERM_PRESENT) == PERM_PRESENT)
				(*num_pages)++;
			if (ptr_page_table_source != ptr_last_located_page_table)
			{
				(*num_tables)++;
				ptr_last_located_page_table = ptr_page_table_source;
			}
		}
		sva += PAGE_SIZE;
	}
}

/*BONUS*/
//=====================================
// 6) CALCULATE REQUIRED FRAMES IN RAM:
//=====================================
// calculate_required_frames:
// calculates the new allocation size required for given address+size,
// we are not interested in knowing if pages or tables actually exist in memory or the page file,
// we are interested in knowing whether they are allocated or not.
uint32 calculate_required_frames(uint32 *page_directory, uint32 sva, uint32 size)
{
	// TODO: [PROJECT MS2 - BONUS] [CHUNK OPERATIONS] calculate_required_frames
	//  Write your code here, remove the panic and write your code
	//  panic("calculate_required_frames() is not implemented yet...!!");
	uint32 required_frames = 0, va, eva;
	va = ROUNDDOWN(sva, PAGE_SIZE), eva = ROUNDUP(sva + size, PAGE_SIZE);
	while (va < eva)
	{
		uint32 *ptr_page_table_source = NULL;
		struct FrameInfo *ptr_frame_info = get_frame_info(page_directory, va, &ptr_page_table_source);
		if (ptr_frame_info == NULL)
			required_frames++;
		va += PAGE_SIZE;
	}
	va = ROUNDDOWN(sva, PAGE_SIZE * NPTENTRIES), eva = ROUNDUP(sva + size, PAGE_SIZE * NPTENTRIES);
	while (va < eva)
	{
		uint32 *ptr_page_table_source = NULL;
		int ret = get_page_table(page_directory, va, &ptr_page_table_source);
		if (ret == TABLE_NOT_EXIST)
			required_frames++;
		va += PAGE_SIZE * NPTENTRIES;
	}
	return required_frames;
}

//=================================================================================//
//===========================END RAM CHUNKS MANIPULATION ==========================//
//=================================================================================//

/*******************************/
/*[2] USER CHUNKS MANIPULATION */
/*******************************/

//======================================================
/// functions used for USER HEAP (malloc, free, ...)
//======================================================

//=====================================
// 1) ALLOCATE USER MEMORY:
//=====================================
void allocate_user_mem(struct Env *e, uint32 virtual_address, uint32 size)
{
	// Write your code here, remove the panic and write your code
	panic("allocate_user_mem() is not implemented yet...!!");
}

//=====================================
// 2) FREE USER MEMORY:
//=====================================
void free_user_mem(struct Env *e, uint32 virtual_address, uint32 size)
{
	// TODO: [PROJECT MS3] [USER HEAP - KERNEL SIDE] free_user_mem
	//  Write your code here, remove the panic and write your code
	panic("free_user_mem() is not implemented yet...!!");

	// This function should:
	// 1. Free ALL pages of the given range from the Page File
	// 2. Free ONLY pages that are resident in the working set from the memory
	// 3. Removes ONLY the empty page tables (i.e. not used) (no pages are mapped in the table)
}

//=====================================
// 2) FREE USER MEMORY (BUFFERING):
//=====================================
void __free_user_mem_with_buffering(struct Env *e, uint32 virtual_address, uint32 size)
{
	// your code is here, remove the panic and write your code
	panic("__free_user_mem_with_buffering() is not implemented yet...!!");

	// This function should:
	// 1. Free ALL pages of the given range from the Page File
	// 2. Free ONLY pages that are resident in the working set from the memory
	// 3. Free any BUFFERED pages in the given range
	// 4. Removes ONLY the empty page tables (i.e. not used) (no pages are mapped in the table)
}

//=====================================
// 3) MOVE USER MEMORY:
//=====================================
void move_user_mem(struct Env *e, uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
{
	// TODO: [PROJECT MS3 - BONUS] [USER HEAP - KERNEL SIDE] move_user_mem
	// your code is here, remove the panic and write your code
	panic("move_user_mem() is not implemented yet...!!");

	// This function should move all pages from "src_virtual_address" to "dst_virtual_address"
	// with the given size
	// After finished, the src_virtual_address must no longer be accessed/exist in either page file
	// or main memory

	/**/
}

//=================================================================================//
//========================== END USER CHUNKS MANIPULATION =========================//
//=================================================================================//
