/* *********************************************************** */
/* MAKE SURE PAGE_WS_MAX_SIZE = 9 */
/* *********************************************************** */

#include <inc/lib.h>

char arr[PAGE_SIZE*12];
char* ptr = (char* )0x0801000 ;
char* ptr2 = (char* )0x0804000 ;

void _main(void)
{
	
//	cprintf("envID = %d\n",envID);

	
	

	//("STEP 0: checking Initial WS entries ...\n");
	{
		if( ROUNDDOWN(myEnv->__uptr_pws[0].virtual_address,PAGE_SIZE) !=   0x200000)  	panic("INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[1].virtual_address,PAGE_SIZE) !=   0x201000)  panic("INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[2].virtual_address,PAGE_SIZE) !=   0x202000)  panic("INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[3].virtual_address,PAGE_SIZE) !=   0x203000)  panic("INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[4].virtual_address,PAGE_SIZE) !=   0x204000)  panic("INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[5].virtual_address,PAGE_SIZE) !=   0x205000)  panic("INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[6].virtual_address,PAGE_SIZE) !=   0x800000)  panic("INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[7].virtual_address,PAGE_SIZE) !=   0x801000)  panic("INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[8].virtual_address,PAGE_SIZE) !=   0xeebfd000)  panic("INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( myEnv->page_last_WS_index !=  0)  										panic("INITIAL PAGE WS last index checking failed! Review size of the WS..!!");
	}

	int freePages = sys_calculate_free_frames();
	int usedDiskPages = sys_pf_calculate_allocated_pages();

	//Reading (Not Modified)
	char garbage1 = arr[PAGE_SIZE*11-1] ;
	char garbage2 = arr[PAGE_SIZE*12-1] ;

	//Writing (Modified)
	int i ;
	for (i = 0 ; i < PAGE_SIZE*10 ; i+=PAGE_SIZE/2)
	{
		arr[i] = -1 ;
		/*2016: this BUGGY line is REMOVED el7! it overwrites the KERNEL CODE :( !!!*/
		//*ptr = *ptr2 ;
		/*==========================================================================*/
		//always use pages at 0x801000 and 0x804000
		*ptr2 = *ptr ;
		ptr++ ; ptr2++ ;

	}

	//===================

	//cprintf("Checking PAGE CLOCK algorithm... \n");
	{
		if( ROUNDDOWN(myEnv->__uptr_pws[0].virtual_address,PAGE_SIZE) !=  0xeebfd000)  panic("Page clock algo failed.. trace it by printing WS before and after page fault");
		if( ROUNDDOWN(myEnv->__uptr_pws[1].virtual_address,PAGE_SIZE) !=  0x80c000)  panic("Page clock algo failed.. trace it by printing WS before and after page fault");
		if( ROUNDDOWN(myEnv->__uptr_pws[2].virtual_address,PAGE_SIZE) !=  0x803000)  panic("Page clock algo failed.. trace it by printing WS before and after page fault");
		if( ROUNDDOWN(myEnv->__uptr_pws[3].virtual_address,PAGE_SIZE) !=  0x804000)  panic("Page clock algo failed.. trace it by printing WS before and after page fault");
		if( ROUNDDOWN(myEnv->__uptr_pws[4].virtual_address,PAGE_SIZE) !=  0x809000)  panic("Page clock algo failed.. trace it by printing WS before and after page fault");
		if( ROUNDDOWN(myEnv->__uptr_pws[5].virtual_address,PAGE_SIZE) !=  0x80a000)  panic("Page clock algo failed.. trace it by printing WS before and after page fault");
		if( ROUNDDOWN(myEnv->__uptr_pws[6].virtual_address,PAGE_SIZE) !=  0x80b000)  panic("Page clock algo failed.. trace it by printing WS before and after page fault");
		if( ROUNDDOWN(myEnv->__uptr_pws[7].virtual_address,PAGE_SIZE) !=  0x800000)  panic("Page clock algo failed.. trace it by printing WS before and after page fault");
		if( ROUNDDOWN(myEnv->__uptr_pws[8].virtual_address,PAGE_SIZE) !=  0x801000)  panic("Page clock algo failed.. trace it by printing WS before and after page fault");

		if(myEnv->page_last_WS_index != 2) panic("wrong PAGE WS pointer location");

	}

	cprintf("Congratulations!! test PAGE replacement [CLOCK Alg.] is completed successfully.\n");
	return;
}
