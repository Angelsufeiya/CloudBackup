// 这个demo封装一个udpsocket类，向外提供简单的接口实现套接字编程

#include <iostream>
#include <cstdio>
#include <string>
#include <unistd.h>	// close接口
#include <stdlib.h>	// atio接口
#include <netinet/in.h>	// 地址结构定义
#include <arpa/inet.h>	// 字节序转换接口
#include <sys/socket.h>	// 套接字接口

class UdpSocket{
public:
	UdpSocket():_sockfd(-1){
	}
	// 1.创建套接字
	bool Socket(){
		_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(_sockfd < 0){
			perror("socket error");
			return false;
		}
		return true;
	}
	// 2.为套接字绑定地址信息
	bool Bind(const std::string &ip, uint32_t port){
		// 1.定义IPv4地址结构
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = inet_addr(ip.c_str());
		// 2.绑定地址
		socklen_t len = sizeof(struct sockaddr_in);
		// bind(描述符, 统一地址结构sockaddr*, 地址信息长度)
		int ret = bind(_sockfd, (struct sockaddr*)&addr, len);
		if(ret < 0){
			perror("bind error");
			return false;
		}
		return true;
	}
	// 3.发送数据
	bool Send(const std::string &data, const std::string &ip, uint16_t port){
		// sendto(描述符，数据，长度，选项，对端地址，地址长度)
		// 1.定义对端地址信息的IPv4地址结构
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = inet_addr(ip.c_str());
		// 2.向这个地址发送数据
		socklen_t len = sizeof(struct sockaddr_in);
		int ret = sendto(_sockfd, data.c_str(), data.size(), 0, (struct sockaddr*)&addr, len);
		if(ret < 0){
			perror("sendto error");
			return false;
		}
		return true;
	}
	// 输入型参数使用 const 引用；输出型参数使用 指针；输入输出型使用引用
	// 4.接收数据
	bool Recv(std::string *buf, std::string *ip = NULL, uint16_t *port = NULL){
		// recvfrom(描述符，缓冲区，数据长度，选项，对端地址，地址长度)
		struct sockaddr_in addr;	// 用于获取发送端地址信息
		socklen_t len = sizeof(struct sockaddr_in);	// 指定地址长度以及获取实际地址长度
		int ret;
		char tmp[4096] = {0};	// 临时用于存放数据的缓冲区
		ret = recvfrom(_sockfd, tmp, 4096, 0, (struct sockaddr*)&addr, &len);
		if(ret < 0){
			perror("recvfrom error");
			return -1;
		}
		buf->assign(tmp, ret);	// 给buf申请ret大小的空间，从tmp中拷贝ret长度的数据进去
		// 为了接口灵活，用户若不想获取地址信息，则不再转换获取
		// 只有当用户想要获取地址的时候，这时候传入缓冲区，我们将数据写入进去
		if(ip != NULL){
			*ip = inet_ntoa(addr.sin_addr);	// 将网络字节序整数IP地址转换为字符串地址，返回
		}
		if(port != NULL){
			*port = ntohs(addr.sin_port);
		}
		return true;
	}
	// 5.关闭套接字
	void Close(){
		close(_sockfd);
		_sockfd = -1;
		return ;
	}
private:
	// 贯穿全文的套接字描述符
	int _sockfd;
	
};

#define CHECK_RET(q) if((q) == false){ return -1; }

// 客户端要给服务端发送数据，那么就需要知道服务端的地址信息
// 因此通过程序运行参数传入服务端的地址信息
int main(int argc, char *argv[]){
	if(argc != 3){
		printf("em: ./udp_cli 192.168.122.132 9000\n");
		return -1;
	}
	// argv[0] = ./udp_cli;
	// argv[1] = 192.168.122.132
	// argv[2] = 9000
	std::string ip_addr = argv[1];	// 服务端地址信息
	uint16_t port_addr = atoi(argv[2]);

	UdpSocket sock;
	CHECK_RET(sock.Socket());	// 创建套接字
	//CHECK_RET(sock.Bind());	// 绑定地址信息
	while(1){
		// 获取一个标准输入的数据，进行发送
		std::cout << "client say: ";
		fflush(stdout);
		std::string buf;
		std::cin >> buf;	// 获取标准输入的数据
		sock.Send(buf, ip_addr, port_addr);	// 向指定的主机进程发送buf数据
		buf.clear();	// 清空buf缓冲区
		sock.Recv(&buf);	// 因为本身客户端就知道服务端的地址，因此不需要再获取了
		std::cout << "server say: " << buf << std::endl;
	}
	sock.Close();

	return 0;
}
