#include "scheduler.h"
#include "process.h"
#include "switch.h"
#include "mmu/mallocate.h"
#include "core/cmos.h"

#include <stddef.h>

process_node_t *init = NULL;
process_node_t *current=NULL;
process_node_t *running_task = NULL;
int pidnum=0;
int scheduler_init(void)
{
	init = malloc(sizeof(process_node_t));
	
	init->pcb.pid = 0;
	init->pcb.runtime=__INT_MAX__;
	init->pcb.status=1;
	init->pcb.type=0;
	init->next = NULL;
	init ->pre = NULL;
	current=init;
	running_task = init;
	return 0;
}

int schedule()
{
	/*if(running_task != NULL && running_task->next != NULL)
	{
		running_task = running_task->next;
	}
	//printf("%X;%X\n",running_task,running_task->next);
	return 0;*/
	int min=__INT_MAX__;
	process_node_t *p = NULL;
	p=init;
	while (p!=NULL)
	{
		if (p->pcb.runtime<min)
		{
			min=p->pcb.runtime;
		}
		p=p->next;
	}
	p=init;
	while (p!=NULL)
	{
		if (p->pcb.runtime==min)
		{
			running_task=p;
			p->pcb.status=1;
			break;
		}
		p=p->next;
	}
	if (running_task!=init)
	{
		running_task->pcb.runtime--;
	}
	if (running_task->pcb.runtime==0)
	{
		kill();
	}
	
}

int fork()
{
	process_node_t *new_task=NULL;
	if(init != NULL)
	{
		new_task= malloc(sizeof(process_node_t));
		cmos_rtc_t rtc = cmos_read_rtc();
		pidnum++;
		new_task->pcb.pid =pidnum;
		new_task->pcb.runtime=(rtc.seconds%5)+1;
		new_task->pcb.status=0;
		new_task->pcb.type=1;
		new_task->next=NULL;
		new_task->pre=current;
		current->next=new_task;
		current=new_task;
	}
	return 0;
}

int kill(void)
{
	running_task->pre->next=running_task->next;
	if (running_task->next!=NULL)
	{
		running_task->next->pre=running_task->pre;
	}
	running_task=init;
	printf("kill one process\n");
	return 0;
}
int sample_task(void)
{
	process_node_t *p=NULL;
	p=init;
	while(p != NULL){
		printf("-pid:%d\n",p->pcb.pid);
		printf("-runtime:%d\n",p->pcb.runtime);
		printf("-status:%d\n",p->pcb.status);
		printf("-type:%d\n",p->pcb.type);
		p=p->next;
	}
	return 0;
}