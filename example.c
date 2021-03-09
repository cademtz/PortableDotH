#include "include/portable/pthread.h"
#include <stdio.h>

pthread_code_t PTHREAD_CALL PrintThread(void* Args);

int main()
{
	pthread_t t1, t2;
	pthread_code_t code;

	pthread_create(&t1, 0, &PrintThread, (void*)"#1 Test");
	msleep(500); // 500 ms, or 0.5 seconds
	pthread_create(&t2, 0, &PrintThread, (void*)"Test #2");

	pthread_join(t1, &code);
	printf("Exit code 1: %p\n", (void*)code);

	pthread_join(t2, &code);
	printf("Exit code 2: %p\n", (void*)code);

	puts("Both threads have finished.");
	return 0;
}

pthread_code_t PTHREAD_CALL PrintThread(void* Args)
{
	const char* str = (const char*)Args;
	for (int i = 0; i < 5; i++)
	{
		puts(str);
		sleep(1);
	}

	return (pthread_code_t)(( str[0] == '#' ) ? 0 : -1);
}