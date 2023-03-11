#include<WinSock2.h>
#include<WS2tcpip.h>
#include <iostream>
//导入WinSock2.h、WS2tcpip.h实现部分
#pragma comment(lib,"ws2_32.lib")

#define MAXLink 128
#define ServerPort 8888
#define BufSize 128

int main() {
	//打开网络库
	WSADATA data{};
	if (WSAStartup(MAKEWORD(2, 2), &data) == SOCKET_ERROR) {
		perror("WSAStartup err:");
		std::cout << WSAGetLastError() << std::endl;
		return -1;
	}

	//确定为TCP协议栈，最后参数为0也行，前两个已经确认了
	SOCKET serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSock == INVALID_SOCKET) {
		std::cout << "server socket err:" << WSAGetLastError() << std::endl;
		WSACleanup();
		return -1;
	}

	sockaddr_in servAddr;
	//bind 确认协议族
	servAddr.sin_family = AF_INET;
	//htons 即host to net short，将16位本地字节序转化为网络字节序，如端口
	servAddr.sin_port = htons(ServerPort);
	//htonl host to net long 将32位字节序转化为网络字节序
	//使用更安全的
	inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr.S_un.S_addr);
	if (bind(serverSock, (sockaddr*)&servAddr, sizeof(sockaddr)) == SOCKET_ERROR) {
		std::cout << "bind func err:" << WSAGetLastError() << std::endl;
		WSACleanup();
		return -1;
	}

	//listen函数
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
	// send函数和recv函数必须检查
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
			std::cout << "客户端强制断开" << std::endl;
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
	//调用网卡的设备模块也清理
	WSACleanup();
	system("pause");
	return 0;
}