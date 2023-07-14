/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
 ~ Author        Lin Weintraub                                   ~
 * Version       0.0.1                                           *
 ~ Date          26/10/22                                        ~
 * Description   Scheduler                                       *
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */

#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */

#include "scheduler.h"
#include "task.h"

enum status {success, fail};

static int CmpTaskFunction(void *param_a, void *param_b);
static int IsMatchUID(void *param_a, void *param_b);

struct scheduler
{
    priority_queue_t *tasks;
    int stop;
};

scheduler_t *SchedulerCreate(void)
{
	scheduler_t *scheduler = malloc(sizeof(scheduler_t));
	if (NULL == scheduler)
	{
		return NULL;
	}
	
	scheduler->tasks = PQCreate(&CmpTaskFunction); 
	
	if (NULL == scheduler->tasks)
	{
		free(scheduler); scheduler = NULL;
		return NULL;
	}
	
	scheduler->stop = 0;
	
	return (scheduler);
}

 
static int CmpTaskFunction(void *task_1, void *task_2)
{
	task_t *temp_task_1 = NULL;
	task_t *temp_task_2 = NULL;
	
	assert(task_1);
	assert(task_2);
	
	temp_task_1 = (task_t *)task_1;
	temp_task_2 = (task_t *)task_2;
			
    return(TaskGetExecutionTime(temp_task_1) - TaskGetExecutionTime(temp_task_2));
}

void SchedulerDestroy(scheduler_t *scheduler)
{
	assert(scheduler);

	SchedulerClear(scheduler);
	PQDestroy(scheduler->tasks);
	
	free(scheduler);
	scheduler = NULL;
}

uid_ty SchedulerAddTask(scheduler_t *scheduler, action_func_t op_func, void *param, time_t execution_time, size_t interval_time)
{
	task_t *new_task = TaskCreate(op_func, execution_time, interval_time, param);
	int enqueue_status = 0;
	uid_ty task_uid = bad_uid;

	assert(scheduler);
	assert(0 <= execution_time);
	assert(op_func);
	assert(param);
	
	if (NULL == new_task)
	{
		return bad_uid;
	}
	
	task_uid = GetUid(new_task);
	if (1 == UIDIsEqual(task_uid, bad_uid))
	{
		return bad_uid;
	}	
	
	enqueue_status = PQEnqueue(scheduler->tasks, new_task);
	if (1 == enqueue_status)
	{
		TaskDestroy(new_task);
		return bad_uid;
	}
	
	return(task_uid);
}

int SchedulerRemoveTask(scheduler_t *scheduler, uid_ty task_uid)
{
	task_t *remove_task = NULL;
	void *temp_pq_task = PQErase(scheduler->tasks, &IsMatchUID, &task_uid);
	
	assert(scheduler);
	assert(0 == UIDIsEqual(task_uid, bad_uid));
	
	if (NULL == temp_pq_task)
	{
		return fail;
	}
	
	remove_task = (task_t *)temp_pq_task;
	TaskDestroy(remove_task);
	
	return success;
}

static int IsMatchUID(void *uid, void *task)
{
	assert(uid);
	assert(task);
	
	return (UIDIsEqual(*(uid_ty *)uid, GetUid(task)));
}

void SchedulerClear(scheduler_t *scheduler)
{
	assert(scheduler);
	
	while (0 != SchedulerSize(scheduler))
	{
		SchedulerRemoveTask(scheduler,GetUid(PQPeek(scheduler->tasks)));
	}
}
 
size_t SchedulerSize(const scheduler_t *scheduler)
{
	assert(scheduler);
	
	return (PQSize(scheduler->tasks));
}

int SchedulerIsEmpty(const scheduler_t *scheduler)
{
	assert(scheduler);
	
	return (PQIsEmpty(scheduler->tasks));
}

int SchedulerRun(scheduler_t *scheduler)
{
	task_t *task_to_run = NULL;
	time_t time_to_run = 0;
	double time_to_sleep = 0;
	time_t curr_time = 0;
	int func_status = 0;
	int enqueue_status = 0;
	scheduler->stop = 0;
	
	while (0 == PQIsEmpty(scheduler->tasks) && (0 == scheduler->stop))
	{	
		curr_time = time(NULL);
		if (-1 == curr_time)
		{
			return -1;
		}
	
		task_to_run = PQDequeue(scheduler->tasks);	
		time_to_run = TaskGetExecutionTime(task_to_run);
		time_to_sleep = difftime(time_to_run, curr_time);

		if (time_to_sleep > 0)
		{
		    sleep(time_to_sleep);
		}

        func_status = TaskRun(task_to_run);

        if (0 == TaskGetIntervalTime(task_to_run))
        {
			TaskDestroy(task_to_run);
			
            continue;
        }
        
		if (1 == func_status)
		{
			SchedulerStop(scheduler);
			TaskDestroy(task_to_run);
			return fail;
		}
		
        TaskSetExecutionTime(task_to_run, TaskGetIntervalTime(task_to_run));
        PQEnqueue(scheduler->tasks, task_to_run);
		if (1 == enqueue_status)
		{
			SchedulerStop(scheduler);
			return fail;
		}
	}
	
	if (1 == scheduler->stop)
	{
		return success;
	}

	return 0;
}

void SchedulerStop(scheduler_t *scheduler)
{
	assert(scheduler);
	
	scheduler->stop = 1;
}
