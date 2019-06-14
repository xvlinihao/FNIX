#ifndef KMAIN_H
#define KMAIN_H

#include <core/boot.h>

#define KERNEL_SYMBO	"*****   ***   *   *   ***   *   *\n"\
						"*        *    **  *    *     * *\n"\
						"*****    *    * * *    *      *\n"\
						"*        *    *  **    *     * *\n"\
						"*       ***   *   *   ***   *   *\n"

#define HELP_MESSAGE	"- help:             show avalible commands\n"\
						"- time:             show UTC time\n"\
						"- memory map:       show memory mapping\n"\
						"- background mode:  enter background mode, press Esc to come back\n"\
						"- fork:             excute fork() function and create a new sample task\n"\
						"- job:              show the running processs\n"\
					    "- ps:               list the processes in ready queue\n"\
					    "- seebmap:          \n"\
					    "- testwrite1:       \n"\
					    "- testread1:        \n"\
					    "- testwrite2:       \n"\
					    "- testread2:        \n"\
					    "- ls:               \n"\
					    "- testcreate:       \n"\
					    "- deletedir:        \n"

#define COM_HELP "help"
#define COM_TIME "time"
#define COM_MMAP "memory map"

#define COM_BGMD "background mode"
#define COM_FORK "fork"
#define COM_JOB "job"
#define COM_PS "ps"

#define COM_BMAP "seebmap"
#define COM_WRITE1 "testwrite1"
#define COM_READ1 "testread1"
#define COM_WRITE2 "testwrite2"
#define COM_READ2 "testread2"		
#define COM_LS "ls"
#define COM_TEST_CREATE "testcreate"
#define COM_DELETE_DIR "deletedir"

typedef enum mode
{
	USER_MODE,
	BACKGROUND_MODE
} mode_t;

void kmain(unsigned long magic, unsigned long addr) __asm__("kmain");

void get_commands(void *buf);

#endif
