// 使用封装的TcpSocket类实例化对象实现tco服务端程序

#include <iostream>
#include "tcpsocket.hpp"

int main(int argc, char *argv[]){
	if(argc != 3){
		printf("em:./tcp_srv 192.168.122.132 9000\n");
		return -1;
	}
	std::string ip = argv[1];
	uint16_t port = std::stoi(argv[2]);	// stoi将字符串转换为数字
	
	TcpSocket lst_sock;
	CHECK_RET(lst_sock.Socket());
	CHECK_RET(lst_sock.Bind(ip, port));
	CHECK_RET(lst_sock.Listen());
	while(1){
		TcpSocket cli_sock;
		bool ret = lst_sock.Accept(&cli_sock);
		if(ret == false){
			continue；
		}
	}
	lst_sock.Close();
	
	return 0;
}
