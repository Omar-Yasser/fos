/*
 * test_commands.c
 *
 *  Created on: Oct 16, 2022
 *      Author: ghada
 */

#include <kern/tests/test_commands.h>
#include <kern/cmd/command_prompt.h>

//define the white-space symbols
#define WHITESPACE "\t\r\n "

int TestAutoCompleteCommand()
{
	cprintf("Automatic Testing of Autocomplete:\n");
	cprintf("\n========================\n");
//	cprintf("========================\n");
//	cprintf("Q2 Test: manually try the test cases in the doc. \n..."
//			"OR, you can do it automatically by un-commenting the code in this function, it should output the same results in the Examples exist in the MS1 ppt\n");
//
//	int retValue = 0;
	int i = 0;

	//CASE1:
	//should execute the kernel_info command
	cprintf("==>Testing now AUTOCOMPLETE for: kernel_info\n");
	char cr0[100] = "kernel_info";
	execute_command(cr0) ;
	cprintf("=================\n\n");

	//CASE2: should print the commands that start with he ---> Shall print (help)
	cprintf("==>Testing now AUTOCOMPLETE for: he\n");
	char cr2[100] = "he";
	execute_command(cr2) ;
	cprintf("=================\n\n");


	//CASE3: should print the commands that start with ru ---> Shall print (rum, rub, rut, run, runall) .. Each in a separate line
	cprintf("==>Testing now AUTOCOMPLETE for: ru\n");
	char cr3[100] = "ru";
	execute_command(cr3) ;
	cprintf("=================\n\n");

	//CASE4: should print unknown command
	cprintf("==>Testing now AUTOCOMPLETE for: smm\n");
	char cr4[100] = "smm";
	execute_command(cr4) ;
	cprintf("=================\n\n");
	return 0;
}


