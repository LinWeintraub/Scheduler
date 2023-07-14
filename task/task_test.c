/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
 ~ Author        Lin Weintraub                                   ~
 * Version       0.0.1                                           *
 ~ Date          25/10/22                                        ~
 * Description   Task                                            *
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */
 
#include <stdio.h> /*printf*/

#include "task.h"

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define CYAN "\033[1;36m"
#define WHITE "\033[0m"

#define TEST(name, actual, expected) \
    printf("%s: %s\n" , name, actual == expected ? GREEN"PASS"WHITE : RED"FAIL"WHITE)
#define TESTNOT(name, actual, expected) \
    printf("%s: %s\n" , name, actual != expected ? GREEN"PASS"WHITE : RED"FAIL"WHITE)

void TestTaskCreate(void);
void TestGetExecutionTime(void);
void TestTaskSetExecutionTime(void);
void TestTaskRun(void);
   
int IntPrint(void *param);


int main(void)
{
	TestTaskCreate();
	TestGetExecutionTime();
	TestTaskSetExecutionTime();
	TestTaskRun();
	
	return 0;
}

void TestTaskCreate(void)
{
    task_t *my_task = TaskCreate(IntPrint, 5, 5, (void *)5);
    uid_ty uid_task = GetUid(my_task);
    
	printf(CYAN"-------------------------------TaskCreate & GetUid Test-------------------------------\n"WHITE);
	
    printf("uid_task.pid: %d\n", uid_task.pid);
    printf("uid_task.time: %li\n", uid_task.time);
    printf("uid_task.counter: %lu\n", uid_task.counter);
    printf("uid_task.ip_address: %s\n", uid_task.ip_address);
    
	TaskDestroy(my_task);
	
	return;
}   

void TestTaskSetExecutionTime(void)
{
	time_t test1; time_t test2;
    task_t *my_task = TaskCreate(IntPrint, 5, 5, (void *)5);
	
	printf(CYAN"-------------------------------TaskSetExecutionTime Test-------------------------------\n"WHITE);
	
	test1 = TaskGetExecutionTime(my_task);
	
	TaskSetExecutionTime(my_task, 10);
	
	test2 = TaskGetExecutionTime(my_task);
	
	printf("Before Update: %s\n" ,ctime(&test1));
	printf("After Update: %s\n" ,ctime(&test2));
	
	TaskDestroy(my_task);
}

void TestGetExecutionTime(void)
{
	time_t test;
    task_t *my_task = TaskCreate(IntPrint, 5, 5, (void *)5);
	
	printf(CYAN"-------------------------------GetExecutionTime Test-------------------------------\n"WHITE);
	
	test = time(NULL);
	printf("The current time is: %s\n" ,ctime(&test));
	
	test = TaskGetExecutionTime(my_task);

	printf("Time to Run test (in 5 seconds): %s\n" ,ctime(&test));
	
	TaskDestroy(my_task);
	
	return;
}


void TestTaskRun(void)
{
    task_t *my_task = TaskCreate(IntPrint, 5, 5, (void *)5);
	
	printf(CYAN"-------------------------------TaskRun Test (print 5)-------------------------------\n"WHITE);

	TaskRun(my_task);
	
	TaskDestroy(my_task);
	
	return;
}

int IntPrint(void *param)
{
	int i = 0;
	(void) param;
	
	for (i = 0; i < 5; ++i)
	{
		printf("%d\n", i);
	}
		
	return 0;
}
