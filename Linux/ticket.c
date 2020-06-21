// 操作售票系统代码 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <pthread.h>

int ticket = 100;
pthread_mutex_t mutex;

void *route(void *arg)
{
	while(1){
		// 加锁一定是只保护临界资源的访问
		pthread_mutex_lock(&mutex);
		if (ticket > 0) {
			// 如果有票就一直枪
			usleep(1000);
			printf("sell ticket:%d\n", ticket);
			ticket--; 
			pthread_mutex_unlock(&mutex);
		} 
		else {
			// 加锁后在任意有可能退出线程的地方都要解锁
			pthread_mutex_unlock(&mutex);
			pthread_exit(NULL);
		}
	}
}

int main(){
	pthread_t tid[4];

	int i, ret;
	// 互斥锁的初始化一定要放在线程创建之前
	pthread_mutex_init(&mutex, NULL);
	for(i = 0; i < 4; i++){
		ret = pthread_create(&tid[i], NULL, route, NULL);
		if(ret != 0){
			printf("thread create error");
			return -1;
		}
	}

	for(i = 0; i < 4; i++){
		pthread_join(tid[i], NULL);
	}
	// 互斥锁的销毁一定是不再使用这个互斥锁
    pthread_mutex_destroy(&mutex);

	return 0;
}
