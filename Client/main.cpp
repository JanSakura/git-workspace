#include"client.h"
#include <iostream>
//µ¼ÈëWinSock2.h¡¢WS2tcpip.hÊµÏÖ²¿·Ö
#pragma comment(lib,"ws2_32.lib")

int main() {
	Client client;
	if (!client.start("127.0.0.1", 8887)) {
		return -1;
	}
	if (!client.recvMsg()) {
		return -1;
	}
	if (!client.sendMsg()) {
		return -1;
	}
	//client.close();
	return 0;
}
