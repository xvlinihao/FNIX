#ifndef SCH_PROCESS_H
#define SCH_PROCESS_H

#include <stdint.h>

typedef struct pcb
{
	uint64_t pid;//进程id
	uint8_t runtime;//运行所需时间
	uint64_t status;//当前状态
	uint64_t type;//进程类型
} pcb_t;

typedef struct process_node
{
	pcb_t pcb;
	struct process_node *next;//指向子进程
	struct process_node *pre;//指向父进程
} process_node_t;
#endif