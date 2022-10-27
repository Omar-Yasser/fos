#
# This makefile system follows the structuring conventions
# recommended by Peter Miller in his excellent paper:
#
#	Recursive Make Considered Harmful
#	http://aegis.sourceforge.net/auug97.pdf
#

V 			:= @
TOP 		:= .
OBJDIR 		:= obj
TOOLPREFIX 	:= i386-elf-
QEMU 		:= qemu-system-i386
PERL		:= perl
IMAGE 		:= $(OBJDIR)/fos.img

CC			:= $(TOOLPREFIX)gcc -m32 -pipe
GCC_LIB 	:= $(shell $(CC) -print-libgcc-file-name)
AS			:= $(TOOLPREFIX)as --32
AR			:= $(TOOLPREFIX)ar
LD			:= $(TOOLPREFIX)ld -m elf_i386
OBJCOPY		:= $(TOOLPREFIX)objcopy
OBJDUMP		:= $(TOOLPREFIX)objdump
NM			:= $(TOOLPREFIX)nm

# Note for migration to new build systems, if this doesn't work with 64 compilers, try adding -m32 to NCC flags
# Native commands
NCC	:= gcc $(CC_VER) -m32 -pipe
TAR	:= gtar
PERL	:= perl

# Cross-compiler fos toolchain
#
# This Makefile will automatically use the cross-compiler toolchain
# installed as 'i386-elf-*', if one exists.  If the host tools ('gcc',
# 'objdump', and so forth) compile for a 32-bit x86 ELF target, that will
# be detected as well.  If you have the right compiler toolchain installed
# using a different name, set GCCPREFIX explicitly in conf/env.mk

# try to infer the correct GCCPREFIX

ifndef GCCPREFIX
GCCPREFIX := $(shell if i386-elf-objdump -i 2>&1 | grep '^elf32-i386$$' >/dev/null 2>&1; \
	then echo 'i386-elf-'; \
	elif objdump -i 2>&1 | grep 'elf32-i386' >/dev/null 2>&1; \
	then echo ''; \
	else echo "***" 1>&2; \
	echo "*** Error: Couldn't find an i386-*-elf version of GCC/binutils." 1>&2; \
	echo "*** Is the directory with i386-elf-gcc in your PATH?" 1>&2; \
	echo "*** If your i386-*-elf toolchain is installed with a command" 1>&2; \
	echo "*** prefix other than 'i386-elf-', set your GCCPREFIX" 1>&2; \
	echo "*** environment variable to that prefix and run 'make' again." 1>&2; \
	echo "*** To turn off this error, run 'gmake GCCPREFIX= ...'." 1>&2; \
	echo "***" 1>&2; exit 1; fi)
endif

# Compiler flags
# -fno-builtin is required to avoid refs to undefined functions in the kernel.
# Only optimize to -O1 to discourage inlining, which complicates backtraces.
CFLAGS	:= -O0 -fgnu89-inline -fno-builtin -I$(TOP) -MD -Wall -Wno-format -Wno-unused -Werror -fno-stack-protector -ggdb -g3

# Linker flags for FOS user programs
ULDFLAGS := -T user/user.ld

# Lists that the */Makefrag makefile fragments will add to
OBJDIRS :=

# Make sure that 'all' is the first target
all:
# TEMPORARILY MODIFY ALL .c FILES INSIDE "kern/*/" SUBDIR TO FORCE BUILDING THEM EACH TIME (el7 :))
# OTHERWISE, ANY CHANGE IN ANY .h FILE IN THE ENTIRE CODE WILL NOT REFLECT ON ANY OF THESE "kern/*/*" FILES
	touch -m kern/cons/console.c
	touch -m kern/cons/printf.c
	touch -m kern/cmd/command_prompt.c
	touch -m kern/cmd/commands.c
	touch -m kern/disk/pagefile_manager.c
	touch -m kern/cpu/kclock.c
	touch -m kern/cpu/sched.c
	touch -m kern/cpu/picirq.c
	touch -m kern/mem/boot_memory_manager.c
	touch -m kern/mem/memory_manager.c
	touch -m kern/mem/shared_memory_manager.c
	touch -m kern/mem/kheap.c
	touch -m kern/mem/paging_helpers.c
	touch -m kern/mem/working_set_manager.c
	touch -m kern/mem/chunk_operations.c
	touch -m kern/proc/user_environment.c
	touch -m kern/proc/priority_manager.c
	touch -m kern/proc/user_programs.c
	touch -m kern/trap/trap.c
	touch -m kern/trap/trapentry.S
	touch -m kern/trap/syscall.c
	touch -m kern/trap/kdebug.c
	touch -m kern/trap/fault_handler.c
	touch -m kern/conc/semaphore_manager.c
	touch -m kern/tests/test_dynamic_allocator.c
	touch -m kern/tests/test_priority.c
	touch -m kern/tests/test_kheap.c
	touch -m kern/tests/utilities.c
	touch -m kern/tests/test_commands.c

# Eliminate default suffix rules
.SUFFIXES:

# Delete target files if there is an error (or make is interrupted)
.DELETE_ON_ERROR:

# make it so that no intermediate .o files are ever deleted
.PRECIOUS: %.o \
 	$(OBJDIR)/boot/%.o \
	$(OBJDIR)/kern/%.o \
	$(OBJDIR)/lib/%.o \
	$(OBJDIR)/fs/%.o \
	$(OBJDIR)/user/%.o

KERN_CFLAGS := $(CFLAGS) -DFOS_KERNEL -gstabs
USER_CFLAGS := $(CFLAGS) -DFOS_USER -gstabs


# Include Makefrags for subdirectories
include boot/Makefrag
include kern/Makefrag
include lib/Makefrag
include user/Makefrag


# Emulators

GDBPORT 	= 26000
QEMUGDB 	= -gdb tcp::$(GDBPORT)
QEMUOPTS 	= -drive file=$(IMAGE),media=disk,format=raw -smp 2 -m 32 $(QEMUEXTRAS)

qemu: all
	$(V)$(QEMU) -serial mon:stdio $(QEMUOPTS)

qemu-gdb: all
	$(QEMU) $(QEMUOPTS) -S $(QEMUGDB)


# For deleting the build

clean:
	rm -rf $(OBJDIR)


# This magic automatically generates makefile dependencies
# for header files included from C source files we compile,
# and keeps those dependencies up-to-date every time we recompile.
# See 'mergedep.pl' for more information.
$(OBJDIR)/.deps: $(foreach dir, $(OBJDIRS), $(wildcard $(OBJDIR)/$(dir)/*.d))
	@mkdir -p $(@D)
	@$(PERL) mergedep.pl $@ $^

-include $(OBJDIR)/.deps


.PHONY: all clean qemu
