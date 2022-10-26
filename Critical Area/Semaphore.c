#include <pthread.h> //mythread
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //유닉스계열
#include <errno.h>
#include <ctype.h>
#include <semaphore.h>

void* mythread_r(void* ptr);
void* mythread_w(void* ptr);

sem_t g_mutex; //전역변수

int
main(int argc, char* argv[])
{
	pthread_t thread1, thread2;
	int nValue = 0; //임계 구역 설정을 위해 하나의 정수형 변수 선언

	sem_init(&g_mutex, 0, 1); // 0으로 초기화

	// start the threads
	pthread_create(&thread1, NULL, *mythread_r, (void*)&nValue);
	pthread_create(&thread2, NULL, *mythread_w, (void*)&nValue);

	// wait for threads to finish
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	sem_destroy(&g_mutex); // 세마포어 해제

	return 0;
}

void*
mythread_r(void* p_Value)
{
	int* p_nValue = (int*)p_Value;

	do
	{
		sem_wait(&g_mutex); //내가 쓰고 잠금, 이미 잠겨 있다면 대기
		if (*p_nValue % 2 == 0) 
		{
			usleep(500000);
			printf("%d is even number!!\n", *p_nValue);
		}
		else
		{
			usleep(500000);
			printf("%d is odd number!!\n", *p_nValue);
		}
		sem_post(&g_mutex); // 다 사용했다는 신호(잠금해제), 
							//반드시 해제해줄 것!!(동적메모리할당의 멜록함수 free로 해제하는 것과 같은 원리)

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
		sem_wait(&g_mutex);
		(*p_nValue)++;
		sem_post(&g_mutex);

		usleep(500000);
	} while (1);

	return p_nValue;
}