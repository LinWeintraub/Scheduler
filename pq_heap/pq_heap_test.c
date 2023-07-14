/* ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ *
 ~ Author        Lin Weintraub                                   ~
 * Version       1.0.0                                           *
 ~ Date          1/12/22                                         ~
 * Description   PQ Heap                                         *
 * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ * ~ */
 
#include <stdio.h>

#include "pq_heap.h"

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define CYAN "\033[1;36m"
#define WHITE "\033[0m"

#define TEST(name, actual, expected) \
    printf("%s: %s\n" , name, actual == expected ? GREEN"PASS"WHITE : RED"FAIL"WHITE)
#define TESTNOT(name, actual, expected) \
    printf("%s: %s\n" , name, actual != expected ? GREEN"PASS"WHITE : RED"FAIL"WHITE)

void TestPQHeapCreate(void);
void TestPQEnqueueDequeuePeekIsEmptySize(void);
void TestPQEraseClear(void);

static int CmpFunc(const void *data_1, const void *data_2);
static int IsMatch(const void *param_a, const void *param_b);

int main(void)
{
	TestPQHeapCreate();	
	TestPQEnqueueDequeuePeekIsEmptySize();
	TestPQEraseClear();
	
	return 0;
}

void TestPQHeapCreate(void)
{
    pq_heap_t *test = PQHeapCreate(CmpFunc);
    printf("~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#\n");
    TESTNOT("PQHeapCreate test1: ",test , NULL);
    
    PQHeapDestroy(test);
}    

void TestPQEnqueueDequeuePeekIsEmptySize(void)
{
	int test_arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int i = 0;
    pq_heap_t *test = PQHeapCreate(CmpFunc);
    
    printf("~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#\n");
    for(i = 0; i < 10; ++i)
    {
        PQHeapEnqueue(test, &test_arr[i]);
    }     
     
    TEST("size test1", PQHeapSize(test), 10);
    TEST("IsEmpty test1", PQHeapIsEmpty(test), 0);

    printf("~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#\n");
    for(i = 0; i < 10; ++i)
    {
        printf("PQDequeue + PQueuePeek test %d", i);
        TEST("", PQHeapPeek(test), &test_arr[i]);
        PQHeapDequeue(test);
    }

    printf("~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#\n");
    TEST("size test2 after deletion", PQHeapSize(test), 0);
    TEST("IsEmpty test2 after deletion", PQHeapIsEmpty(test), 1);
    
    PQHeapDestroy(test);
}

void TestPQEraseClear(void)
{
	int test_arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int i = 0;
    pq_heap_t *test = PQHeapCreate(CmpFunc);
    
    printf("~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#\n");
    for(i = 0; i < 10; ++i)
    {
        PQHeapEnqueue(test, &test_arr[i]);
    } 
    
    PQHeapErase(test, &IsMatch, &test_arr[3]);
    TEST("size test after erase", PQHeapSize(test), 9);    
    
    PQHeapErase(test, &IsMatch, &test_arr[3]);
    TEST("size test after erase fail", PQHeapSize(test), 9);  
    
    PQHeapClear(test);
    TEST("size test after clear", PQHeapSize(test), 0);    
    PQHeapDestroy(test);
}

static int CmpFunc(const void *data_1, const void *data_2)
{
    return (*(int *)data_1 - *(int *)data_2);
}

static int IsMatch(const void *param_a, const void *param_b)
{
	if (*(int *)param_a == *(int *)param_b)
	{
		return 1;
	}
	
	return 0;
}
