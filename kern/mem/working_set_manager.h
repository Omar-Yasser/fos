/*
 * working_set_manager.h
 *
 *  Created on: Oct 11, 2022
 *      Author: HP
 */

#ifndef KERN_MEM_WORKING_SET_MANAGER_H_
#define KERN_MEM_WORKING_SET_MANAGER_H_

#include <inc/environment_definitions.h>

// Page WS helper functions ===================================================
inline uint32 env_page_ws_get_size(struct Env *e);
inline void env_page_ws_invalidate(struct Env* e, uint32 virtual_address);
inline void env_page_ws_set_entry(struct Env* e, uint32 entry_index, uint32 virtual_address);
inline void env_page_ws_clear_entry(struct Env* e, uint32 entry_index);
inline uint32 env_page_ws_get_virtual_address(struct Env* e, uint32 entry_index);
inline uint32 env_page_ws_get_time_stamp(struct Env* e, uint32 entry_index);
inline uint32 env_page_ws_is_entry_empty(struct Env* e, uint32 entry_index);
void env_page_ws_print(struct Env *curenv);


// Table WS helper functions ===================================================
inline uint32 env_table_ws_get_size(struct Env *e);
inline void env_table_ws_invalidate(struct Env* e, uint32 virtual_address);
inline void env_table_ws_set_entry(struct Env* e, uint32 entry_index, uint32 virtual_address);
inline void env_table_ws_clear_entry(struct Env* e, uint32 entry_index);
inline uint32 env_table_ws_get_virtual_address(struct Env* e, uint32 entry_index);
inline uint32 env_table_ws_get_time_stamp(struct Env* e, uint32 entry_index);
inline uint32 env_table_ws_is_entry_empty(struct Env* e, uint32 entry_index);
void env_table_ws_print(struct Env *curenv);

// Change WS Sizes For PRIORITY  =========================================================
void cut_paste_WS(struct WorkingSetElement* newWS, int newSize, struct Env* e);
void double_WS_Size(struct Env* e, int isOneTimeOnly);
void half_WS_Size(struct Env* e, int isImmidiate);

#endif /* KERN_MEM_WORKING_SET_MANAGER_H_ */
