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
	//�������
	WSADATA data{};
	if (WSAStartup(MAKEWORD(2, 2), &data) == SOCKET_ERROR) {
		std::cout << "WSAStartup func err:" << WSAGetLastError() << std::endl;
		return false;
	}

	//ȷ��ΪTCPЭ��ջ��������Ϊ0Ҳ�У�ǰ�����Ѿ�ȷ����
	serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSock == INVALID_SOCKET) {
		std::cout << "server socket err:" << WSAGetLastError() << std::endl;
		WSACleanup();
		return false;
	}

	sockaddr_in servAddr;
	//bind ȷ��Э����
	servAddr.sin_family = AF_INET;
	//htons ��host to net short����16λ�����ֽ���ת��Ϊ�����ֽ�����˿�
	servAddr.sin_port = htons(port);
	//htonl host to net long ��32λ�ֽ���ת��Ϊ�����ֽ���
	//ʹ�ø���ȫ��
	inet_pton(AF_INET, ip, &servAddr.sin_addr.S_un.S_addr);
	if (bind(serverSock, (sockaddr*)&servAddr, sizeof(sockaddr)) == SOCKET_ERROR) {
		std::cout << "bind func err:" << WSAGetLastError() << std::endl;
		WSACleanup();
		return false;
	}

	//listen����
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
			std::cout << "�ͻ���ǿ�ƶϿ�" << std::endl;
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
