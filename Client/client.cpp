#include"client.h"
#include<iostream>
#include<WS2tcpip.h>
Client::Client():servSock(INVALID_SOCKET){}

Client:: ~Client() {
	close();
}
void Client::close() {
	if (servSock != INVALID_SOCKET) {
		closesocket(servSock);
		WSACleanup();
		servSock = INVALID_SOCKET;
	}
}

bool Client::init(const char* ip, unsigned short port) {
	//打开网络库
	WSADATA data{};
	if (WSAStartup(MAKEWORD(2, 2), &data) == SOCKET_ERROR) {
		std::cout << "WSAStartup err :" << WSAGetLastError() << std::endl;
		return false;
	}

	servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (servSock == INVALID_SOCKET) {
		std::cout << "server socket err:" << WSAGetLastError() << std::endl;
		WSACleanup();
		return false;
	}

	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(port);
	inet_pton(AF_INET, ip , &servAddr.sin_addr.S_un.S_addr);
	//connect 函数
	if (connect(servSock, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		std::cout << "connect err:" << WSAGetLastError() << std::endl;
		//closesocket(servSock);
		//WSACleanup();
		return false;
	}
}

bool Client::start(const char* ip, unsigned short port) {
	if (!init(ip, port)) return false;
	return true;
}
bool Client::sendMsg() {
	if (send(servSock, recvBuf, recvRet, 0) == SOCKET_ERROR) {
		std::cout << "send func err:" << WSAGetLastError() << std::endl;
		return false;
	}
}
bool Client::recvMsg() {
	recvRet = recv(servSock, recvBuf, 128, 0);
	if (recvRet == 0) {
		std::cout << "server gracefully drop connection:" << std::endl;
	}
	if (recvRet < 0) {
		if (WSAGetLastError() == 10054) {
			std::cout << "服务器强制断开" << std::endl;
		}
		else {
			std::cout << "recv func err:" << WSAGetLastError() << std::endl;
			return false;
		}
	}
}
