/*
 * test_commands.h
 *
 *  Created on: Oct 16, 2022
 *      Author: ghada
 */

#ifndef KERN_TESTS_TEST_COMMANDS_H_
#define KERN_TESTS_TEST_COMMANDS_H_
#ifndef FOS_KERNEL
# error "This is a FOS kernel header; user programs should not #include it"
#endif

#include <kern/cmd/command_prompt.h>
#include <inc/stdio.h>
#include <inc/string.h>
#include <inc/memlayout.h>
#include <inc/assert.h>
#include <inc/x86.h>

int TestAutoCompleteCommand();


#endif /* KERN_TESTS_TEST_COMMANDS_H_ */
