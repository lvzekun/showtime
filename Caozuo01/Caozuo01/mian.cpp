#define _CRT_SECURE_NO_DEPRECATE//解决出现Sanf错误
//error C4996: 'scanf': This function or variable may be unsafe. Consider using scanf_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*process struct*/
typedef struct node
{
	char name[10];/*name of the process*/
	int prio;     /*priority of the process*/
	int round;    /*time given */
	int cputime;  /*time used*/
	int needtime; /*timed needed*/
	int count;    /*counter*/
	char state;   /*state of the process'R':running,'W'：waiting,'F':finish*/
	struct node *next;/*pointer to next process*/
}PCB;

PCB *finish, *ready, *tail, *run;/*pointer to three processSqueue，tail point to the end of readySqueue*/
int N;/*the number of process*/
/*
function:put the first process in readySqueue into running
void firstin(void)
*/
void firstin(void)
{
	if (ready != NULL)
	{
		run = ready;
		ready = ready->next;
		run->state = 'R';
		run->next = NULL;
	}
	else
	{
		run = NULL;
	}
}
/*
function:output the type of the processCalling
void prt1(char a)
a=='p': priority ，=='r':time round
*/
void prt1(char a)
{
	if ((a == 'P') || (a == 'p'))
	{
		printf(" name  cputime  needtime  priority  state \n");
	}
	else
	{
		printf(" name  cputime  needtime  count  round  state \n");
	}
}
/*
function:output information of one process
void prt2(char a,PCB *p)
char a :a=='p':priority =='r':time round
PCB *p : p point to the processSqueue to be outputed
*/
void prt2(char a, PCB *p)
{
	if (a == 'P' || a == 'p')
	{
		printf(" %s,  %d,  %d,  %d,  %c\n", p->name, p->cputime, p->needtime, p->prio, p->state);
	}
	else
	{
		printf("  %s,  %d,  %d,  %d,  %d,  %c\n", p->name, p->cputime, p->needtime, p->count, p->round, p->state);
	}
}
/*
function: output information of all the processSqueue
void prt(char algo)
char algo :algo=='p': priority ，=='r':time round
*/
void prt(char algo)
{
	PCB *p;
	prt1(algo); //output the type
	if (run != NULL) //if the running pointer not equal NULL ,output the process's information
	{
		prt2(algo, run);
	}
	p = ready;
	while (p != NULL)
	{
		prt2(algo, p);//output the information of the readySqueue
		p = p->next;
	}
	p = finish;
	while (p != NULL)
	{
		prt2(algo, p);//output the information of the finishSqueue
		p = p->next;
	}
}
/*
function:Priority method insert a process into a suqeue
void insert1(PCB *q)
*/
void insert1(PCB *q)
{
	PCB *p, *s, *r;
	int b;
	s = q;
	p = ready;
	r = p;
	b = 1;
	if (s->prio >= ready->prio)
	{
		s->next = ready;
		ready = s;
	}
	else
	{
		while ((p != NULL) && b)
		{
			if (p->prio >= s->prio)
			{
				r = p;
				p = p->next;
			}
			else
			{
				b = 0;
			}
		}
		s->next = p;
		r->next = s;
	}
}
/*
function: time round insert o process into a squeue
void insert2(PCB *q)
*/
void insert2(PCB *q)
{
	tail->next = q;
	tail = q;
	q->next = NULL;
}
/*
function:priority method initialization
void pcreate_task(char algo)
*/
void pcreate_task(char algo, int n)
{
	PCB *p;
	int i, time;
	char na[10];
	ready = NULL;
	finish = NULL;
	run = NULL;
	for (i = 0; i<n; i++)
	{
		p = (PCB*)malloc(sizeof(PCB));
		printf("Enter the name of process %d: ", i);
		scanf("%s", &na);
		getchar();
		printf("Enter the time of process %d: ", i);
		scanf("%d", &time);
		getchar();
		strcpy(p->name, na);
		p->cputime = 0;
		p->needtime = time;
		p->state = 'W';
		p->prio = time;
		if (ready == NULL)
		{
			ready = p;
			ready->next = NULL;
		}
		else
		{
			insert1(p);
		}
	}
	printf("Output the waiting processes information\n");
	prt(algo);
	firstin();
}
/*
function:time round initialization
void rcreate_task(char algo)
*/
void rcreate_task(char algo, int n)
{
	PCB *p;
	int i, time;
	char na[10];
	ready = NULL;
	finish = NULL;
	run = NULL;
	for (i = 0; i<n; i++)
	{
		p = (PCB*)malloc(sizeof(PCB));
		printf("Enter the name of process i: ", i);
		scanf("%s", &na);
		getchar();
		printf("Enter the time of process i: ", i);
		scanf("%d", &time);
		getchar();
		strcpy(p->name, na);
		p->cputime = 0;
		p->needtime = time;
		p->count = 0;
		p->state = 'W';
		p->round = 2;
		if (ready != NULL)
		{
			insert2(p);
		}
		else
		{
			p->next = ready;
			ready = p;
			tail = p;
		}
	}
	printf("Output the waiting processes information\n");
	prt(algo);
	run = ready;
	ready = ready->next;
	run->state = 'R';
}
/*
function:priority method calling
void priority(char algo)
char algo:type sign :'P':priority 'R': time round
*/

void priority(char algo)
{
	while (run != NULL)
	{
		run->cputime += 1;
		run->needtime -= 1;
		run->prio -= 3;
		if (run->needtime == 0)
		{
			run->next = finish;
			finish = run;
			run->state = 'F';
			run = NULL;
			firstin();
		}
		else
		{
			if ((ready != NULL) && (run->prio<ready->prio))
			{
				run->state = 'W';
				insert1(run);
				run = NULL;
				firstin();
			}
		}
		// prt(algo);
	}
	prt(algo);
}
/*
function:time round calling
void roundrun(char algo)
*/
void roundrun(char algo)
{
	while (run != NULL)
	{
		run->cputime = run->cputime + 1;
		run->needtime = run->needtime - 1;
		run->count = run->count + 1;
		if (run->needtime == 0)
		{
			run->next = finish;
			finish = run;
			run->state = 'F';
			run = NULL;
			if (ready != NULL)
			{
				firstin();
			}
		}
		else
		{
			if (run->count == run->round)
			{
				run->count = 0;
				if (ready != NULL)
				{
					run->state = 'W';
					insert2(run);
					firstin();
				}
			}
		}
		prt(algo);
	}
}


/*main */


int main()
{
	char algo;
	printf("(**p**表示：按优先数调度算法:);**r**表示（时间片转轮法：操作）");
	printf("Choose the type of attemper P:priority R:timeround : ");
	scanf("%c", &algo);
	getchar();
	printf("Please enter the number of processes N（进程数量）: ");
	scanf("%d", &N);
	getchar();
	if ((algo == 'P') || (algo == 'p'))
	{
		pcreate_task(algo, N);
		priority(algo);
	}
	else if ((algo == 'r') || (algo == 'R'))
	{
		rcreate_task(algo, N);
		roundrun(algo);
	}
	return 1;
}