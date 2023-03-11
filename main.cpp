#include<WinSock2.h>
#include<WS2tcpip.h>
#include <iostream>
//����WinSock2.h��WS2tcpip.hʵ�ֲ���
#pragma comment(lib,"ws2_32.lib")

#define MAXLink 128
#define ServerPort 8888
#define BufSize 128

int main() {
	//�������
	WSADATA data{};
	if (WSAStartup(MAKEWORD(2, 2), &data) == SOCKET_ERROR) {
		perror("WSAStartup err:");
		std::cout << WSAGetLastError() << std::endl;
		return -1;
	}

	//ȷ��ΪTCPЭ��ջ��������Ϊ0Ҳ�У�ǰ�����Ѿ�ȷ����
	SOCKET serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSock == INVALID_SOCKET) {
		std::cout << "server socket err:" << WSAGetLastError() << std::endl;
		WSACleanup();
		return -1;
	}

	sockaddr_in servAddr;
	//bind ȷ��Э����
	servAddr.sin_family = AF_INET;
	//htons ��host to net short����16λ�����ֽ���ת��Ϊ�����ֽ�����˿�
	servAddr.sin_port = htons(ServerPort);
	//htonl host to net long ��32λ�ֽ���ת��Ϊ�����ֽ���
	//ʹ�ø���ȫ��
	inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr.S_un.S_addr);
	if (bind(serverSock, (sockaddr*)&servAddr, sizeof(sockaddr)) == SOCKET_ERROR) {
		std::cout << "bind func err:" << WSAGetLastError() << std::endl;
		WSACleanup();
		return -1;
	}

	//listen����
	if (listen(serverSock, MAXLink) == -1) {
		std::cout << "listen func err:" << WSAGetLastError() << std::endl;
		closesocket(serverSock);
		WSACleanup();
		return -1;
	}
	
	SOCKET clientSock = accept(serverSock, nullptr, nullptr);
	if (clientSock == INVALID_SOCKET) {
		std::cout << "client accept func err:" << WSAGetLastError() << std::endl;
		closesocket(serverSock);
		WSACleanup();
		return -1;
	}

	const char* msg = "hello world";
	char recvBuf[BufSize]{};
	// send������recv����������
	if (send(clientSock, msg, strlen(msg), 0) < 0) {
		std::cout << "send func err:" << WSAGetLastError() << std::endl;
		closesocket(serverSock);
		closesocket(clientSock);
		WSACleanup();
		return -1;
	}

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
			closesocket(serverSock);
			closesocket(clientSock);
			WSACleanup();
			return -1;
		}
	}

	std::cout << recvBuf << std::endl;
	
	closesocket(serverSock);
	closesocket(clientSock);
	//�����������豸ģ��Ҳ����
	WSACleanup();
	system("pause");
	return 0;
}