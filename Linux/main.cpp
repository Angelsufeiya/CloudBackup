#include <iostream>
#include "select.hpp"

int main(int argc, char *argv[]){
	if(argc != 3){
		printf("usage: ./main ip port\n");
		return -1;
	}
	std::string srv_ip = argv[1];
	uint16_t srv_port = std::stoi(argv[2]);

	TcpSocket lst_sock;
	CHECK_RET(lst_sock.Socket());
	CHECK_RET(lst_sock.Bind(srv_ip, srv_port));
	CHECK_RET(lst_sock.Listen());

	Select s;
	s.Add(lst_sock);
	while(1){
		std::vector<TcpSocket> list;
		bool ret = s.Wait(&list);
		if(ret == false){
			return -1;
		}
		for(auto sock : list){
			// 遍历就绪的TcpSocket进行操作：获取新连接/接收数据
			if(sock.GetFd() == lst_sock.GetFd()){
				TcpSocket new_sock;
				ret = lst_sock.Accept(&new_sock);
				if(ret == false){
				  	continue;
				}
				s.Add(new_sock);
			}
			else{
				std::string buf;
				ret = sock.Recv(&buf);
				if(ret == false){
					sock.Close();
					continue;
				}
				printf("client say: %s\n", buf.c_str());
				buf.clear();
				std::cout << "server say:";
				std::cin >> buf;
				ret = sock.Send(buf);
				if(ret == false){
					sock.Close();
					continue;
				}
			}	
		}
	}
	lst_sock.Close();

	return 0;
}
