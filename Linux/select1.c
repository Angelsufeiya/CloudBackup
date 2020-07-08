#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/select.h>

int main(){
	// 对标准输入进行监控--标准输入描述符(0)
	
	// 1.定义指定时间的描述符集合
	fd_set rfds;
	FD_ZERO(&rfds);	// 初始化清空集合
	FD_SET(0, &rfds);	// 将0号描述符添加到集合中
	struct timeval tv;
	tv.tv_sec = 3;
	tv.tv_usec = 0;

	while(1){
		printf("开始监控\n");
		// select(maxfd +1, 可读事件集合，可写事件集合，异常事件集合，超时时间)
		int ret = select(0+1, &rfds, NULL, NULL, &tv);
		if(ret < 0){
			perror("select error");
			return -1;
		}else if(ret == 0){
			printf("wait timeout\n");
			continue;
		}
		if(FD_ISSET(0, &rfds)){	// 判断描述符是否在集合中判断是否就绪了事件
			printf("准备从标准输入读取数据...\n");
			char buf[1024] = { 0 };
			int ret = read(0, buf, 1023);
			if(ret < 0){
				perror("read error");
				FD_CLR(0, &rfds);	// 移除描述符从集合中
				return -1;
			}
			printf("read buf:[%s]\n", buf);
		}
	}
	
	return 0;
}
