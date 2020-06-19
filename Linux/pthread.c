#include <unistd.h> // sleep函数
#include <stdlib.h> // exit函数
#include <stdio.h> // 输入输出函数库
#include <string.h> // strerror函数
#include <pthread.h> // 线程函数库

void *thread1(void *arg){
	printf("thread 1 returning ... \n"); 
	int *p = (int*)malloc(sizeof(int)); 
	*p = 1;
	return (void*)p;
}

void *thread2(void *arg){
	printf("thread 2 exiting ...\n"); 
	int *p = (int*)malloc(sizeof(int)); 
	*p = 2;
	pthread_exit((void*)p);
}

void *thread3(void *arg){
	while (1){ //
		printf("thread 3 is running ...\n"); 
		sleep(1);
	}
    return NULL;
}

int main(){
    pthread_t tid;
    void *ret;

    // thread 1 return
    pthread_create(&tid, NULL, thread1, NULL);
    pthread_join(tid, &ret);
    printf("thread return, thread id %lu, return code:%d\n", tid, *(int*)ret);
	free(ret);

    // thread 2 exit
    pthread_create(&tid, NULL, thread2, NULL);
    pthread_join(tid, &ret);
    printf("thread return, thread id %lu, return code:%d\n", tid, *(int*)ret);
    free(ret);

    // thread 3 cancel by other
    pthread_create(&tid, NULL, thread3, NULL);
    sleep(3);
    pthread_cancel(tid);
    pthread_join(tid, &ret);
    if (ret ==  PTHREAD_CANCELED)
        printf("thread return, thread id %lu, return code:PTHREAD_CANCELED\n", tid);
	else
		printf("thread return, thread id %lu, return code:NULL\n", tid);

	return 0;
}

