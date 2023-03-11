#include"server.h"
#include<iostream>
#include<WS2tcpip.h>
#include<assert.h>
#define MAXLink 128

Server::Server():serverSock(INVALID_SOCKET),clientSock(INVALID_SOCKET) {}

Server::~Server() {
	close();
}

void Server::close() {
	if (serverSock != INVALID_SOCKET) {
		closesocket(serverSock);
		if(clientSock!=INVALID_SOCKET) closesocket(clientSock);
		WSACleanup();
		serverSock = INVALID_SOCKET;
		clientSock = INVALID_SOCKET;
	}
}

bool Server::init(const char* ip, unsigned short port) {
	//打开网络库
	WSADATA data{};
	if (WSAStartup(MAKEWORD(2, 2), &data) == SOCKET_ERROR) {
		std::cout << "WSAStartup func err:" << WSAGetLastError() << std::endl;
		return false;
	}

	//确定为TCP协议栈，最后参数为0也行，前两个已经确认了
	serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSock == INVALID_SOCKET) {
		std::cout << "server socket err:" << WSAGetLastError() << std::endl;
		WSACleanup();
		return false;
	}

	sockaddr_in servAddr;
	//bind 确认协议族
	servAddr.sin_family = AF_INET;
	//htons 即host to net short，将16位本地字节序转化为网络字节序，如端口
	servAddr.sin_port = htons(port);
	//htonl host to net long 将32位字节序转化为网络字节序
	//使用更安全的
	inet_pton(AF_INET, ip, &servAddr.sin_addr.S_un.S_addr);
	if (bind(serverSock, (sockaddr*)&servAddr, sizeof(sockaddr)) == SOCKET_ERROR) {
		std::cout << "bind func err:" << WSAGetLastError() << std::endl;
		WSACleanup();
		return false;
	}

	//listen函数
	if (listen(serverSock, MAXLink) == -1) {
		std::cout << "listen func err:" << WSAGetLastError() << std::endl;
		closesocket(serverSock);
		WSACleanup();
		return false;
	}
}
bool Server::start(const char* ip, unsigned short port) {
	if (!init(ip, port)) {
		return false;
	}
	assert(serverSock != INVALID_SOCKET);
	return true;
}
bool Server::acceptClient() {
	clientSock = accept(serverSock, nullptr, nullptr);
	if (clientSock == INVALID_SOCKET) {
		std::cout << "client accept func err:" << WSAGetLastError() << std::endl;
		return false;
	}
}
bool Server::sendMsg(const char* msg) {
	if (send(clientSock, msg, strlen(msg), 0) < 0) {
		std::cout << "send func err:" << WSAGetLastError() << std::endl;
		//closesocket(serverSock);
		//closesocket(clientSock);
		//WSACleanup();
		return false;
	}
}
bool Server::recvMsg() {
	int recvRet = recv(clientSock, recvBuf, BufSize, 0);
	if (recvRet == 0) {
		std::cout << "client gracefully drop connection:" << std::endl;
	}
	if (recvRet < 0) {
		if (WSAGetLastError() == 10054) {
			std::cout << "客户端强制断开" << std::endl;
		}
		else {
			std::cout << "recv func err:" << WSAGetLastError() << std::endl;
			//closesocket(serverSock);
			//closesocket(clientSock);
			//WSACleanup();
			return false;
		}
	}
}

const char* Server::getRecvBuf() {
	return recvBuf;
}
