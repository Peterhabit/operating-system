#include <pthread.h> //mythread
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //유닉스계열
#include <errno.h>
#include <ctype.h>

void* mythread_r(void* ptr);
void* mythread_w(void* ptr);

int
main(int argc, char* argv[])
{
	pthread_t thread1, thread2;
	int nValue = 0; //임계 구역 설정을 위해 하나의 정수형 변수 선언

	// start the threads
	pthread_create(&thread1, NULL, *mythread_r, (void*)&nValue);
	pthread_create(&thread2, NULL, *mythread_w, (void*)&nValue);

	// wait for threads to finish
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	return 0;
}

void*
mythread_r(void* p_Value)
{
	int* p_nValue = (int*)p_Value;

	do
	{
		if (*p_nValue % 2 == 0) 
		{
			usleep(500000);
			printf("%d is even number!!\n", *p_nValue); //<-출력할려고 할 때
																									//mythread_w로 인하여 +1되어 출력되어 오류발생
		}
		else
		{
			usleep(500000);
			printf("%d is odd number!!\n", *p_nValue);
		}
		usleep(500000);
	} while (1);

	return p_nValue;
}

void*
mythread_w(void* p_Value)
{
	int* p_nValue = (int*)p_Value;

	do
	{
		(*p_nValue)++;

		usleep(500000);
	} while (1);

	return p_nValue;
}