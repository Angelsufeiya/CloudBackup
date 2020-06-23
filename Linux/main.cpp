#include "threadpool.hpp"
#include <unistd.h>

void test_func(int data){
	int sec = (data % 3) + 1;
	printf("tid:%p -- get data:%d , sleep:%d", pthread_self(), data, sec);
	sleep(sec);
}

int tmp_func(int data){
	printf("tif:%p -- tmp_func\n", pthread_self());
	sleep(1);
}

int main(){
	ThreadPool pool;
	for(int i = 0; i < 10; i++){
		ThreadTask task;
		if(i % 2 == 0)
			task.SetTask(i, test_func);
		else
			task.SetTask(i, tmp_func);

		pool.TaskPush(task);
	}
	sleep(1000);
	return 0;
}
