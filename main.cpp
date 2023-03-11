#include"server.h"
#include <iostream>
//导入WinSock2.h、WS2tcpip.h实现部分
#pragma comment(lib,"ws2_32.lib")

int main() {
	Server server;
	if (!server.start("127.0.0.1", 8887)) {
		return -1;
	}
	if (!server.acceptClient()) {
		return -1;
	}
	const char* msg = "hello world";

	if (!server.sendMsg(msg)) {
		return -1;
	}
	if (!server.recvMsg()) {
		return -1;
	}
	std::cout << server.getRecvBuf() << std::endl;
	//server.close();
	system("pause");
	return 0;
}