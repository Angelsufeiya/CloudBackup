// 使用封装的TcpSocket类实例化对象实现tco服务端程序

#include <iostream>
#include <sstream>
#include <stdlib.h>
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
			continue;
		}

		std::string http_req;
		cli_sock.Recv(&http_req);
		printf("req:[%s]\n", http_req.c_str());

		// 响应-首行（版本/状态码/描述）-描述（Content-Length）-空行-正文
		std::string body = "<html><body><h1>Hello, world!</h1></body></html>";
		std::string blank = "\r\n";
		std::stringstream header;
		header << "Content - Length: " << body.size() << "\r\n";
		header << "Content-Type: text/html\r\n";
		std::string first_line = "HTTP/1.1 200 OK\r\n";

		cli_sock.Send(first_line);
		cli_sock.Send(header.str());
		cli_sock.Send(blank);
		cli_sock.Send(body); 
		cli_sock.Close();
	}
	lst_sock.Close();
	
	return 0;
}
