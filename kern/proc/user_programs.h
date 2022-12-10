/*
 * user_programs.h
 *
 *  Created on: Oct 12, 2022
 *      Author: HP
 */

#ifndef KERN_USER_PROGRAMS_H_
#define KERN_USER_PROGRAMS_H_
#ifndef FOS_KERNEL
# error "This is a FOS kernel header; user programs should not #include it"
#endif

extern int NUM_USER_PROGS;

#define DECLARE_START_OF(binary_name)  \
	extern uint8 _binary_obj_user_##binary_name##_start[];

#define PTR_START_OF(binary_name) ( \
	(uint8*) _binary_obj_user_##binary_name##_start \
	)

//=========================================================
struct UserProgramInfo {
	const char *name;
	const char *desc;
	uint8* ptr_start;
};

struct UserProgramInfo*  get_user_program_info(char* user_program_name);
struct UserProgramInfo* get_user_program_info_by_env(struct Env* e);

///===================================================================================
/// To add FOS support for new user program, just add the appropriate lines like below

//The input for any DECLARE_START_OF macro must be the ".c" filename of the user program
DECLARE_START_OF(fos_helloWorld)
DECLARE_START_OF(fos_add)
DECLARE_START_OF(fos_alloc)
DECLARE_START_OF(fos_input)
DECLARE_START_OF(game)
DECLARE_START_OF(fos_static_data_section)
DECLARE_START_OF(fos_data_on_stack)
DECLARE_START_OF(quicksort_heap)
DECLARE_START_OF(fos_fibonacci)
DECLARE_START_OF(fos_factorial)
DECLARE_START_OF(mergesort_leakage);
DECLARE_START_OF(mergesort_noleakage);
DECLARE_START_OF(quicksort_noleakage);

DECLARE_START_OF(tst_placement)
DECLARE_START_OF(tst_placement_1);
DECLARE_START_OF(tst_placement_2);
DECLARE_START_OF(tst_placement_3);
DECLARE_START_OF(tst_page_replacement_LRU_Lists_1);
DECLARE_START_OF(tst_page_replacement_LRU_Lists_2);
DECLARE_START_OF(tst_page_replacement_stack_LRU_Lists);
DECLARE_START_OF(tst_page_replacement_alloc)
DECLARE_START_OF(tst_page_replacement_clock)
DECLARE_START_OF(tst_page_replacement_lru)
DECLARE_START_OF(tst_page_replacement_stack)
DECLARE_START_OF(tst_invalid_access)
DECLARE_START_OF(tst_buffer_1)
DECLARE_START_OF(tst_buffer_2)
DECLARE_START_OF(tst_buffer_2_slave)
DECLARE_START_OF(tst_buffer_3)
DECLARE_START_OF(tst_quicksort_freeHeap)
DECLARE_START_OF(tst_semaphore_1master);
DECLARE_START_OF(tst_semaphore_1slave);
DECLARE_START_OF(tst_semaphore_2master);
DECLARE_START_OF(tst_semaphore_2slave);

DECLARE_START_OF(tst_air);
DECLARE_START_OF(tst_air_clerk);
DECLARE_START_OF(tst_air_customer);

//2022
DECLARE_START_OF(tst_malloc_0);

//2015
DECLARE_START_OF(tst_free_1);
DECLARE_START_OF(tst_free_2);
DECLARE_START_OF(tst_free_3);
DECLARE_START_OF(tst_malloc_1);
DECLARE_START_OF(tst_malloc_2);
DECLARE_START_OF(tst_malloc_3);
DECLARE_START_OF(tst_nextfit);
DECLARE_START_OF(tst_best_fit_1);
DECLARE_START_OF(tst_best_fit_2);
DECLARE_START_OF(tst_first_fit_1);
DECLARE_START_OF(tst_first_fit_2);
DECLARE_START_OF(tst_first_fit_3);
DECLARE_START_OF(tst_worstfit);
DECLARE_START_OF(heap_program);

DECLARE_START_OF(tst_page_replacement_FIFO_1);
DECLARE_START_OF(tst_page_replacement_FIFO_2);
DECLARE_START_OF(tst_page_replacement_mod_clock);

DECLARE_START_OF(tst_sharing_1);
DECLARE_START_OF(tst_sharing_2master);
DECLARE_START_OF(tst_sharing_2slave1);
DECLARE_START_OF(tst_sharing_2slave2);
DECLARE_START_OF(tst_sharing_3);
DECLARE_START_OF(tst_sharing_4);
DECLARE_START_OF(tst_sharing_5_master);
DECLARE_START_OF(tst_sharing_5_slave);
DECLARE_START_OF(tst_sharing_5_slaveB1);
DECLARE_START_OF(tst_sharing_5_slaveB2);

DECLARE_START_OF(tst_realloc_1);
DECLARE_START_OF(tst_realloc_2);
DECLARE_START_OF(tst_realloc_3);

DECLARE_START_OF(tst_freeRAM);
//DECLARE_START_OF(tst_freeRAM_2);
DECLARE_START_OF(tst_freeing_stack);
DECLARE_START_OF(arrayOperations_Master);
DECLARE_START_OF(arrayOperations_quicksort);
DECLARE_START_OF(arrayOperations_mergesort);
DECLARE_START_OF(arrayOperations_stats);
DECLARE_START_OF(MidTermEx_Master);
DECLARE_START_OF(MidTermEx_ProcessA);
DECLARE_START_OF(MidTermEx_ProcessB);

DECLARE_START_OF(ef_fos_fibonacci);
DECLARE_START_OF(ef_fos_factorial);
DECLARE_START_OF(ef_fos_add);
DECLARE_START_OF(tst_envfree1);
DECLARE_START_OF(ef_mergesort_noleakage);
DECLARE_START_OF(ef_mergesort_leakage);
DECLARE_START_OF(tst_envfree2);
DECLARE_START_OF(ef_tst_sharing_1);
DECLARE_START_OF(ef_tst_sharing_2master);
DECLARE_START_OF(ef_tst_sharing_2slave1);
DECLARE_START_OF(tst_envfree3);
DECLARE_START_OF(ef_tst_semaphore_1master);
DECLARE_START_OF(ef_tst_semaphore_1slave);
DECLARE_START_OF(tst_envfree4);
DECLARE_START_OF(ef_tst_sharing_4);
DECLARE_START_OF(tst_envfree5_1);
DECLARE_START_OF(ef_tst_sharing_4);
DECLARE_START_OF(ef_tst_sharing_5_master);
DECLARE_START_OF(ef_tst_sharing_5_slave);
DECLARE_START_OF(ef_tst_sharing_5_slaveB1);
DECLARE_START_OF(ef_tst_sharing_5_slaveB2);
DECLARE_START_OF(tst_envfree5_2);
DECLARE_START_OF(ef_tst_sharing_1);
DECLARE_START_OF(ef_MidTermEx_Master);
DECLARE_START_OF(tst_envfree6);


#endif /* KERN_USER_PROGRAMS_H_ */
