#include <cstdio>
#include <vector>
#include <sys/epoll.h>
#include "tcpsocket.hpp"

#define MAX_TIMEOUT 3000
class Epoll{
public:
	Epoll():_epfd(-1){
		// 1.创建epoll句柄
		_epfd = epoll_create(1);
		if(_epfd < 0){
			perror("epoll create error");
			exit(-1);
		}
	}

	bool Add(TcpSocket &sock){
		// 2.添加描述符监控事件信息
		// 获取描述符
		int fd = sock.GetFd();
		// 定义描述符对应的事件结构体
		// EPOLLIN - 可读事件  EPOLLOUT - 可写事件
		struct epoll_event ev;
		ev.events = EPOLLIN;
		ev.data.fd = fd;
		// 添加到内核中
		// epoll_ctl(epoll句柄，操作类型，监控标舒服，描述符事件结构);
		int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev);
		if(ret < 0){
			perror("epoll ctl add error");
			return false;
		}
		return true;
	}

	bool Del(TcpSocket &sock){
		int fd = sock.GetFd();
		int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL);
		if(ret < 0){
			perror("epoll ctl del error");
			return false;
		}
		return true;
	}

	bool Wait(std::vector<TcpSocket> *list, int timeout = MAX_TIMEOUT){
		// 1.开始监控
		struct epoll_event evs[10];
		int nfds = epoll_wait(_epfd, evs, 10, timeout);
		if(nfds < 0){
			perror("epoll wait error");
			return false;
		}
		else if(nfds == 0){
			printf("epoll wait timeout\n");
			list->clear();
			return true;
		}
		// 2.监控调用返回后，为每一个就绪的描述符组织TcpSock对象
		for(int i = 0l i < nfds; i++){
			if(evs[i]. events & EPOLLIN){	// 可读事件的操作
				TcpSocket sock;
				sock.SetFd(evs[i].data.fd);
				// 3.将TcpSocket对象添加到list中，进行返回
				list->push_back(sock);
			}
			// else if() 其他事件的判断操作
		}
	}
private:
	int _epfd;
};
