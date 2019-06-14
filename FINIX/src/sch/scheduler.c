#include "scheduler.h"
#include "process.h"
#include "switch.h"
#include "mmu/mallocate.h"
#include "core/cmos.h"

#include <stddef.h>
#include "kernel/kmain.h"
/*
@para: init:内核0号进程，系统开机创建，第一个用户进程的父进程
		current：当前进程链表的尾部，用于插入链表
		running_task：当前使用cpu的进程
*/
process_node_t *init = NULL;
process_node_t *current=NULL;
process_node_t *running_task = NULL;
int pidnum=0;
extern mode_t mode;

/*
@func:创建内核进程
@para:无
*/
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
/*
@func:对就绪队列的进程进行调度，本系统采用的是剥夺的sjf算法
@para:无
*/
int schedule()
{
	int min=__INT_MAX__;
	process_node_t *p = NULL;
	p=init;
	while (p!=NULL)//遍历就绪队列，获得当前最短运行时间
	{
		if (p->pcb.runtime<min)
		{
			min=p->pcb.runtime;
		}
		p=p->next;
	}
	p=init;
	while (p!=NULL)//找到运行时间最短的进程，让它运行
	{
		if (p->pcb.runtime==min)
		{
			running_task->pcb.status=0;
			running_task=p;
			p->pcb.status=1;
			break;
		}
		p=p->next;
	}
	if (running_task!=init)//用户进程运行时间--
	{
		running_task->pcb.runtime--;
	}
	if (running_task->pcb.runtime==0)//用户进程运行时间为0后将它杀死
	{
		kill();
	}
	
}
/*
@func:创建一个子进程
@para:无
*/
int fork()
{
	process_node_t *new_task=NULL;
	if(init != NULL)
	{
		new_task= malloc(sizeof(process_node_t));
		cmos_rtc_t rtc = cmos_read_rtc();
		pidnum++;
		new_task->pcb.pid =pidnum;
		new_task->pcb.runtime=(rtc.seconds%5)+1;//对当前时间余除运算获取伪随机数作为runtime
		new_task->pcb.status=0;
		new_task->pcb.type=1;
		new_task->next=NULL;
		new_task->pre=current;
		current->next=new_task;
		current=new_task;
	}
	printf("create new task, pid:%d\n",pidnum);
	//printf("running_task:  -pid:%d;  -runtime:%d;  -status:%d;  -type:%d\n",current->pcb.pid,current->pcb.runtime,current->pcb.status,current->pcb.type);
	return pidnum;
}
/*
@func:当进程运行结束后将其移出就绪队列
@para:无
*/
int kill(void)
{
	int killed_pid = running_task->pcb.pid;
	running_task->pre->next=running_task->next;
	if (running_task->next!=NULL)
	{
		running_task->next->pre=running_task->pre;
	}
	else{
		current=running_task->pre;
	}
	running_task=init;
	if(mode == BACKGROUND_MODE)
		printf("kill one process, pid:%d\n",killed_pid);
	return 0;
}
int sample_task(void)
{
	process_node_t *p = NULL;
	p = running_task;
	if(p != NULL && mode == BACKGROUND_MODE)
		printf("running_task:  -pid:%d;  -runtime:%d;  -status:%d;  -type:%d\n",p->pcb.pid,p->pcb.runtime,p->pcb.status,p->pcb.type);
	return 0;
}
/*
@func:显示占用cpu的进程
*/
int job(void)
{
	process_node_t *p = NULL;
	p = running_task;
	if(p != NULL)
		printf("running_task:  -pid:%d;  -runtime:%d;  -status:%d;  -type:%d\n",p->pcb.pid,p->pcb.runtime,p->pcb.status,p->pcb.type);
	return 0;
}
/*
@func:显示就绪队列所有进程
*/
int ps(void)
{
	process_node_t *p=NULL;
	p=init;
	while(p != NULL){
		printf("-pid:%d;  -runtime:%d;  -status:%d;  -type:%d\n",p->pcb.pid,p->pcb.runtime,p->pcb.status,p->pcb.type);
		p=p->next;
	}
	return 0;
}