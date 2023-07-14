/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
 ~ Author        Lin Weintraub                                   ~
 * Version       0.0.1                                           *
 ~ Date          26/10/22                                        ~
 * Description   Scheduler                                       *
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */
 
#include <stdio.h> /* printf */
#include <stdlib.h> /* FILE */
#include <string.h> /*strcmp */

#include "scheduler.h"

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define CYAN "\033[1;36m"
#define WHITE "\033[0m"

#define TEST(name, actual, expected) \
    printf("%s: %s\n" , name, actual == expected ? GREEN"PASS"WHITE : RED"FAIL"WHITE)
#define TESTNOT(name, actual, expected) \
    printf("%s: %s\n" , name, actual != expected ? GREEN"PASS"WHITE : RED"FAIL"WHITE)

void TestSchedulerCreate(void);
void TestSchedulerAddTaskRemoveIsEmptySize(void);
void TestSchedulerRun(void);

int IntPrint(void *param);
int StopFunc(void *param);
int ReadAndWrite(void *file_name);
int Cmp(const char* str1, const char* str2);
int Exit(void);

struct task_file
{
	char *string;
	int (*Cmp)(const char *str1, const char *str2);
	int (*StopFunc)(void *param); 
	int (*Exit)(void); 
};

int main(void)
{
	TestSchedulerCreate();
	TestSchedulerAddTaskRemoveIsEmptySize();
	TestSchedulerRun();
	
	return 0;
}

void TestSchedulerCreate(void)
{
	scheduler_t *test = SchedulerCreate();
    
    TESTNOT("SchedulerCreate test: ",test , NULL);
    
	SchedulerDestroy(test);
}

void TestSchedulerAddTaskRemoveIsEmptySize(void)
{
	scheduler_t *test = SchedulerCreate();
	
	uid_ty add_task = SchedulerAddTask(test, IntPrint, (void *)5, 5, 10);
	
	printf(CYAN"----------------------Test SchedulerAddTask/Remove/IsEmpty--------------------\n"WHITE);
	
	TEST("IsEmpty Test after AddTask", SchedulerIsEmpty(test), 0);	
	TEST("Size Test after AddTask", SchedulerSize(test), 1);	
	
	SchedulerRemoveTask(test, add_task);
	
	TEST("IsEmpty Test after Remove", SchedulerIsEmpty(test), 1);	
	
	SchedulerDestroy(test);
}

void TestSchedulerRun(void)
{
	int param = 1;
	int param2 = 2;
	int param3 = 3;
	
	scheduler_t *test = SchedulerCreate();
	
	SchedulerAddTask(test, StopFunc, test, time(NULL) + 10, 0);
	SchedulerAddTask(test, IntPrint, &param, time(NULL) + 0, 1);								
	SchedulerAddTask(test, IntPrint, &param2, time(NULL) + 3, 2);								
	SchedulerAddTask(test, IntPrint, &param3, time(NULL) + 5, 3);
	/*SchedulerAddTask(test, ReadAndWrite, test, time(NULL) + 15, 0);*/

										
	printf(CYAN"----------------------Stop Task In 10sec--------------------\n"WHITE);
	SchedulerRun(test);

	printf(CYAN"-------------No Stop Task, Should stop by 'return 1'-------------\n"WHITE);
	
	TEST("IsEmpty Test after AddTask", SchedulerIsEmpty(test), 0);	
	TEST("Size Test after AddTask", SchedulerSize(test), 4);	
	SchedulerRun(test);
	
	SchedulerDestroy(test);		
	
	return;							
}

int StopFunc(void *param)
{
	scheduler_t *scheduler = (scheduler_t *)param;
	SchedulerStop(scheduler);
	
	return 1;
}

int IntPrint(void *param)
{
	static int count = 0;
	int a = *(int *)param;

	printf("%d\n" ,a);
	
	++count;	
	
	if (count > 20)
	{
		return 1;
	}
	
	return 0;
}
/*
int ReadFromAFile(void *param)
{
    char *string;
	scheduler_t *scheduler = (scheduler_t *)param;
	
    FILE *reader = fopen("my_file", "r");
    
    if (NULL == reader)
    {
        return 1;
    }
    
    while((string = fgets(reader)) != EOF)
    {
        if(strcmp(string, "stop") == 0)
        {
            SchedulerStop(scheduler);
        }
    }

    fclose(reader);
    
    return 0;
}

int WriteToAFile(void *param)
{
	char buffer[256];
	FILE *source = fopen("my_file", "w");
	if(NULL == source)
	{
		return 1;
	}
	
	while((buffer = fgets((char *)param)) != EOF)
    {	
		if(puts(buffer, source) == EOF)
		{
			return 1;
		}
    }
    
    fclose(source);

	return 0;
}

int ReadAndWrite(void *param)
{    
	char buffer[256];
	scheduler_t *scheduler = (scheduler_t *)param;
	FILE *source = fopen("my_file", "r");
	FILE *destination = fopen("my_file", "w");
	
    if (source == NULL)
    {
        return 1;
    }

    if (destination == NULL)
    {
        fclose(source);
        return 1;
    }

    if (NULL != fgets(buffer, sizeof(buffer), stdin))
    {
        fwrite(&buffer, sizeof(char), 1, destination);
        
        if(strcmp(buffer, "stop") == 0)
        {
            SchedulerStop(scheduler);
        }
    }

    fclose(source);
    fclose(destination);
    
    return 0;
}

int ReadAndWrite(void *file_name)
{
	struct task_file task_file[2];
	char buffer[256];
	int Cmp_int = 0;
	int i = 0;
	
	task_file[0].string = "-stop";
	task_file[0].Cmp = Cmp;
	task_file[0].StopFunc = StopFunc;
	task_file[0].Exit = Exit;
	
	task_file[1].string = "-exit";
	task_file[1].Cmp = Cmp;
	task_file[1].Exit = Exit;
	task_file[1].StopFunc = StopFunc;
	
	while(1)
	{
		if (NULL != fgets(buffer, sizeof(buffer), stdin))
		{
			for(i = 0; i < 2; i++)
			{
				Cmp_int = task_file[i].Cmp(buffer, task_file[i].string);
				if(Cmp_int == 0)
				{
					task_file[i].Operation((char *)file_name, buffer);
					break;
				}
				
				else
				{
					return 1;
				}
			}
		}
	}

	return 0;
}

int Cmp(const char* str1, const char* str2)
{
    return strncmp(str1, str2, strlen(str2));
}

int Exit(void)
{
	(void)file_name;
	(void)str;
	
    exit(0);
    
    return 0;
}*/
