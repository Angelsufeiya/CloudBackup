// 使用封装的TcpSocket类实例化对象实现tco服务端程序

#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include "tcpsocket.hpp"

void sigcb(int signo){
	// 当子进程退出的时候就会向父进程发送SIGCHLD信号，回掉这个函数
	// waitpid返回值>0表示处理了一个退出的子进程
	// waitpid<=0 表示没有退出的子进程
	while(waitpid(-1, 0, WNOHANG) > 0);	// 一次回调循环将退出的子进程全部处理
}

int main(int argc, char *argv[]){
	if(argc != 3){
		printf("em:./tcp_srv 192.168.122.132 9000\n");
		return -1;
	}
	std::string ip = argv[1];
	uint16_t port = std::stoi(argv[2]);	// stoi将字符串转换为数字
	
	signal(SIGCHLD, sigcb);
	TcpSocket lst_sock;
	CHECK_RET(lst_sock.Socket());
	CHECK_RET(lst_sock.Bind(ip, port));
	CHECK_RET(lst_sock.Listen());
	while(1){
		TcpSocket cli_sock;
		std::string cli_ip;
		uint16_t cli_port;
		bool ret = lst_sock.Accept(&cli_sock, &cli_ip, &cli_port);
		if(ret == false){
			continue;
		}
		printf("new connect: [%s:%d]\n", cli_ip.c_str(), cli_port);
		// ------------------------------------------------------
		pid_t pid = fork();
		if(pid == 0){	// 子进程复制父进程 - 数据独有，代码共享
			// 让子进程处理与客户端通信
			while(1){
				// 通过新获取的通信套接字与客户端进行通信
				std::string buf;
				if(cli_sock.Recv(&buf) == false){
					cli_sock.Close();	// 通信套接字接收数据出错，关闭的是通信套接字
					exit(0);
				}
				printf("client:[%s:%d] say:%s\n", &cli_ip[0], cli_port, &buf[0]);
				std::cout << "server say:";
				fflush(stdout);
				buf.clear();
				std::cin >> buf;

				if(cli_sock.Send(buf) == false){
					cli_sock.Close();
					exit(0);
				}
			}
			cli_sock.Close();
			exit(0);
		}
		// 父子进程数据独有，都会具有cli_sock,但是父进程并不通信
		cli_sock.Close();	// 这个关闭对子进程没有影响，数据各自有一份
	}
	lst_sock.Close();
	
	return 0;
}
