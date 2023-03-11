#include<WinSock2.h>
#include<WS2tcpip.h>
#include <iostream>
//����WinSock2.h��WS2tcpip.hʵ�ֲ���
#pragma comment(lib,"ws2_32.lib")

int main() {
	//�������
	WSADATA data{};
	if (WSAStartup(MAKEWORD(2, 2), &data) == SOCKET_ERROR) {
		std::cout << "WSAStartup err :" << WSAGetLastError() << std::endl;
		return -1;
	}

	SOCKET servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (servSock == INVALID_SOCKET) {
		std::cout << "server socket err:" << WSAGetLastError() << std::endl;
		WSACleanup();
		return -1;
	}

	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(8888);
	inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr.S_un.S_addr);
	//connect ����
	if (connect(servSock, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		std::cout << "connect err:" << WSAGetLastError() << std::endl;
		closesocket(servSock);
		WSACleanup();
		return -1;
	}
	//recv & send func
	char recvBuf[128]{};
	int recvLen = recv(servSock, recvBuf, 128, 0);
	if (recvLen == 0) {
		std::cout << "server gracefully drop connection:" << std::endl;
	}
	if (recvLen < 0) {
		if (WSAGetLastError() == 10054) {
			std::cout << "������ǿ�ƶϿ�" << std::endl;
		}
		else {
			std::cout << "recv func err:" << WSAGetLastError() << std::endl;
			closesocket(servSock);
			WSACleanup();
			return -1;
		}
	}

	if (send(servSock, recvBuf, recvLen, 0) == SOCKET_ERROR ) {
		std::cout << "send func err:" << WSAGetLastError() << std::endl;
		closesocket(servSock);
		WSACleanup();
		return -1;
	}

	closesocket(servSock);
	WSACleanup();
	return 0;
}