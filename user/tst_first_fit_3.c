/* *********************************************************** */
/* MAKE SURE PAGE_WS_MAX_SIZE = 1000 */
/* *********************************************************** */
#include <inc/lib.h>

void _main(void)
{

	sys_set_uheap_strategy(UHP_PLACE_FIRSTFIT);

	//Initial test to ensure it works on "PLACEMENT" not "REPLACEMENT"
	{
		uint8 fullWS = 1;
		for (int i = 0; i < myEnv->page_WS_max_size; ++i)
		{
			if (myEnv->__uptr_pws[i].empty)
			{
				fullWS = 0;
				break;
			}
		}
		if (fullWS) panic("Please increase the WS size");
	}

	int envID = sys_getenvid();

	/*Dummy malloc to enforce the UHEAP initializations*/
	malloc(0);
	/*=================================================*/

	int Mega = 1024*1024;
	int kilo = 1024;
	void* ptr_allocations[20] = {0};
	int freeFrames ;
	int usedDiskPages;
	//[1] Allocate all
	{
		//Allocate Shared 1 MB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[0] = smalloc("x", 1*Mega, 1);
		if (ptr_allocations[0] != (uint32*)USER_HEAP_START) panic("Returned address is not correct. check the setting of it and/or the updating of the shared_mem_free_address");
		if ((freeFrames - sys_calculate_free_frames()) !=  256+1+2) panic("Wrong allocation: make sure that you allocate the required space in the user environment and add its frames to frames_storage");
		if( (sys_pf_calculate_allocated_pages() - usedDiskPages) !=  0) panic("Wrong page file allocation: ");

		//Allocate 1 MB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[1] = malloc(1*Mega-kilo);
		if ((uint32) ptr_allocations[1] != (USER_HEAP_START + 1*Mega)) panic("Wrong start address for the allocated space... ");
		//if ((freeFrames - sys_calculate_free_frames()) != 256 ) panic("Wrong allocation: ");
		if( (sys_pf_calculate_allocated_pages() - usedDiskPages) !=  256) panic("Wrong page file allocation: ");
		if ((freeFrames - sys_calculate_free_frames()) != 1 ) panic("Wrong allocation: ");

		//Allocate 1 MB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[2] = malloc(1*Mega-kilo);
		if ((uint32) ptr_allocations[2] != (USER_HEAP_START + 2*Mega)) panic("Wrong start address for the allocated space... ");
		//if ((freeFrames - sys_calculate_free_frames()) != 256 ) panic("Wrong allocation: ");
		if( (sys_pf_calculate_allocated_pages() - usedDiskPages) !=  256) panic("Wrong page file allocation: ");
		if ((freeFrames - sys_calculate_free_frames()) != 0 ) panic("Wrong allocation: ");

		//Allocate 1 MB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[3] = malloc(1*Mega-kilo);
		if ((uint32) ptr_allocations[3] != (USER_HEAP_START + 3*Mega) ) panic("Wrong start address for the allocated space... ");
		//if ((freeFrames - sys_calculate_free_frames()) != 256 ) panic("Wrong allocation: ");
		if( (sys_pf_calculate_allocated_pages() - usedDiskPages) !=  256) panic("Wrong page file allocation: ");
		if ((freeFrames - sys_calculate_free_frames()) != 0 ) panic("Wrong allocation: ");

		//Allocate 2 MB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[4] = malloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[4] != (USER_HEAP_START + 4*Mega)) panic("Wrong start address for the allocated space... ");
		//if ((freeFrames - sys_calculate_free_frames()) != 512 + 1) panic("Wrong allocation: ");
		if( (sys_pf_calculate_allocated_pages() - usedDiskPages) !=  512) panic("Wrong page file allocation: ");
		if ((freeFrames - sys_calculate_free_frames()) != 1) panic("Wrong allocation: ");

		//Allocate Shared 2 MB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[5] = smalloc("y", 2*Mega, 1);
		if (ptr_allocations[5] != (uint32*)(USER_HEAP_START + 6*Mega)) panic("Returned address is not correct. check the setting of it and/or the updating of the shared_mem_free_address");
		if ((freeFrames - sys_calculate_free_frames()) !=  512+1+2) panic("Wrong allocation: make sure that you allocate the required space in the user environment and add its frames to frames_storage");
		if( (sys_pf_calculate_allocated_pages() - usedDiskPages) !=  0) panic("Wrong page file allocation: ");

		//Allocate 3 MB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[6] = malloc(3*Mega-kilo);
		if ((uint32) ptr_allocations[6] != (USER_HEAP_START + 8*Mega)) panic("Wrong start address for the allocated space... ");
		//if ((freeFrames - sys_calculate_free_frames()) != 768 + 1) panic("Wrong allocation: ");
		if( (sys_pf_calculate_allocated_pages() - usedDiskPages) !=  768) panic("Wrong page file allocation: ");
		if ((freeFrames - sys_calculate_free_frames()) != 1) panic("Wrong allocation: ");

		//Allocate Shared 3 MB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[7] = smalloc("z", 3*Mega, 0);
		if (ptr_allocations[7] != (uint32*)(USER_HEAP_START + 11*Mega)) panic("Returned address is not correct. check the setting of it and/or the updating of the shared_mem_free_address");
		if ((freeFrames - sys_calculate_free_frames()) !=  768+2+2) panic("Wrong allocation: make sure that you allocate the required space in the user environment and add its frames to frames_storage");
		if( (sys_pf_calculate_allocated_pages() - usedDiskPages) !=  0) panic("Wrong page file allocation: ");
	}

	//[2] Free some to create holes
	{
		//1 MB Hole
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		free(ptr_allocations[1]);
		//if ((sys_calculate_free_frames() - freeFrames) != 256) panic("Wrong free: ");
		if( (usedDiskPages - sys_pf_calculate_allocated_pages()) !=  256) panic("Wrong page file free: ");
		if ((sys_calculate_free_frames() - freeFrames) != 0) panic("Wrong free: ");

		//2 MB Hole
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		free(ptr_allocations[4]);
		//if ((sys_calculate_free_frames() - freeFrames) != 512) panic("Wrong free: ");
		if( (usedDiskPages - sys_pf_calculate_allocated_pages()) !=  512) panic("Wrong page file free: ");
		if ((sys_calculate_free_frames() - freeFrames) != 0) panic("Wrong free: ");

		//3 MB Hole
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		free(ptr_allocations[6]);
		//if ((sys_calculate_free_frames() - freeFrames) != 768) panic("Wrong free: ");
		if( (usedDiskPages - sys_pf_calculate_allocated_pages()) !=  768) panic("Wrong page file free: ");
		if ((sys_calculate_free_frames() - freeFrames) != 0) panic("Wrong free: ");
	}

	//[3] Allocate again [test first fit]
	{
		//Allocate 512 KB - should be placed in 1st hole
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[8] = malloc(512*kilo - kilo);
		if ((uint32) ptr_allocations[8] != (USER_HEAP_START + 1*Mega)) panic("Wrong start address for the allocated space... ");
		//if ((freeFrames - sys_calculate_free_frames()) != 128) panic("Wrong allocation: ");
		if( (sys_pf_calculate_allocated_pages() - usedDiskPages) !=  128) panic("Wrong page file allocation: ");
		if ((freeFrames - sys_calculate_free_frames()) != 0) panic("Wrong allocation: ");

		//Allocate 1 MB - should be placed in 2nd hole
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[9] = malloc(1*Mega - kilo);
		if ((uint32) ptr_allocations[9] != (USER_HEAP_START + 4*Mega)) panic("Wrong start address for the allocated space... ");
		//if ((freeFrames - sys_calculate_free_frames()) != 256) panic("Wrong allocation: ");
		if( (sys_pf_calculate_allocated_pages() - usedDiskPages) !=  256) panic("Wrong page file allocation: ");
		if ((freeFrames - sys_calculate_free_frames()) != 0) panic("Wrong allocation: ");

		//Allocate 256 KB - should be placed in remaining of 1st hole
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[10] = malloc(256*kilo - kilo);
		if ((uint32) ptr_allocations[10] != (USER_HEAP_START + 1*Mega + 512*kilo)) panic("Wrong start address for the allocated space... ");
		//if ((freeFrames - sys_calculate_free_frames()) != 64) panic("Wrong allocation: ");
		if( (sys_pf_calculate_allocated_pages() - usedDiskPages) !=  64) panic("Wrong page file allocation: ");
		if ((freeFrames - sys_calculate_free_frames()) != 0) panic("Wrong allocation: ");

		//Allocate 2 MB - should be placed in 3rd hole
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[11] = malloc(2*Mega);
		if ((uint32) ptr_allocations[11] != (USER_HEAP_START + 8*Mega)) panic("Wrong start address for the allocated space... ");
		//if ((freeFrames - sys_calculate_free_frames()) != 256) panic("Wrong allocation: ");
		if( (sys_pf_calculate_allocated_pages() - usedDiskPages) !=  512) panic("Wrong page file allocation: ");
		if ((freeFrames - sys_calculate_free_frames()) != 0) panic("Wrong allocation: ");

		//Allocate 4 MB - should be placed in end of all allocations
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[12] = malloc(4*Mega - kilo);
		if ((uint32) ptr_allocations[12] != (USER_HEAP_START + 14*Mega) ) panic("Wrong start address for the allocated space... ");
		//if ((freeFrames - sys_calculate_free_frames()) != 1024 + 1) panic("Wrong allocation: ");
		if( (sys_pf_calculate_allocated_pages() - usedDiskPages) !=  1024) panic("Wrong page file allocation: ");
		if ((freeFrames - sys_calculate_free_frames()) != 2) panic("Wrong allocation: ");
	}

	//[4] Free contiguous allocations
	{
		//1 MB Hole appended to previous 256 KB hole
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		free(ptr_allocations[2]);
		//if ((sys_calculate_free_frames() - freeFrames) != 256) panic("Wrong free: ");
		if( (usedDiskPages - sys_pf_calculate_allocated_pages()) !=  256) panic("Wrong page file free: ");
		if ((sys_calculate_free_frames() - freeFrames) != 0) panic("Wrong free: ");

		//1 MB Hole appended to next 1 MB hole
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		free(ptr_allocations[9]);
		//if ((sys_calculate_free_frames() - freeFrames) != 256) panic("Wrong free: ");
		if( (usedDiskPages - sys_pf_calculate_allocated_pages()) !=  256) panic("Wrong page file free: ");
		if ((sys_calculate_free_frames() - freeFrames) != 0) panic("Wrong free: ");

		//1 MB Hole appended to previous 1 MB + 256 KB hole and next 2 MB hole
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		free(ptr_allocations[3]);
		//if ((sys_calculate_free_frames() - freeFrames) != 256) panic("Wrong free: ");
		if( (usedDiskPages - sys_pf_calculate_allocated_pages()) !=  256) panic("Wrong page file free: ");
		if ((sys_calculate_free_frames() - freeFrames) != 0) panic("Wrong free: ");
	}

	//[5] Allocate again [test first fit]
	{
		//[FIRST FIT Case]
		//Allocate 1 MB + 256 KB - should be placed in the contiguous hole (256 KB + 4 MB)
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[13] = malloc(1*Mega + 256*kilo - kilo);
		if ((uint32) ptr_allocations[13] != (USER_HEAP_START + 1*Mega + 768*kilo)) panic("Wrong start address for the allocated space... ");
		//if ((freeFrames - sys_calculate_free_frames()) != 512+32) panic("Wrong allocation: ");
		if( (sys_pf_calculate_allocated_pages() - usedDiskPages) !=  256+64) panic("Wrong page file allocation: ");
		if ((freeFrames - sys_calculate_free_frames()) != 0) panic("Wrong allocation: ");

		//Allocate Shared 4 MB [should be placed at the end of all allocations
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[14] = smalloc("w", 4*Mega, 0);
		if (ptr_allocations[14] != (uint32*)(USER_HEAP_START + 18*Mega)) panic("Returned address is not correct. check the setting of it and/or the updating of the shared_mem_free_address");
		if ((freeFrames - sys_calculate_free_frames()) !=  1024+2+2) panic("Wrong allocation: make sure that you allocate the required space in the user environment and add its frames to frames_storage");
		if( (sys_pf_calculate_allocated_pages() - usedDiskPages) !=  0) panic("Wrong page file allocation: ");

		//Get shared of 3 MB [should be placed in the remaining part of the contiguous (256 KB + 4 MB) hole
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[15] = sget(envID, "z");
		if (ptr_allocations[15] != (uint32*)(USER_HEAP_START + 3*Mega)) panic("Returned address is not correct. check the setting of it and/or the updating of the shared_mem_free_address");
		if ((freeFrames - sys_calculate_free_frames()) !=  0+0+0) panic("Wrong allocation: make sure that you allocate the required space in the user environment and add its frames to frames_storage");
		if( (sys_pf_calculate_allocated_pages() - usedDiskPages) !=  0) panic("Wrong page file allocation: ");

		//Get shared of 1st 1 MB [should be placed in the remaining part of the 3 MB hole
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[16] = sget(envID, "x");
		if (ptr_allocations[16] != (uint32*)(USER_HEAP_START + 10*Mega)) panic("Returned address is not correct. check the setting of it and/or the updating of the shared_mem_free_address");
		if ((freeFrames - sys_calculate_free_frames()) !=  0+0+0) panic("Wrong allocation: make sure that you allocate the required space in the user environment and add its frames to frames_storage");
		if( (sys_pf_calculate_allocated_pages() - usedDiskPages) !=  0) panic("Wrong page file allocation: ");

	}
	cprintf("Congratulations!! test FIRST FIT allocation (3) completed successfully.\n");

	return;
}
