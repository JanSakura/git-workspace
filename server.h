#ifndef _SERVER_
#define _SERVER_
#include<WinSock2.h>

#define BufSize 128

class Server {
public:
	Server();
	virtual ~Server();
	void close();
	bool start(const char*ip,unsigned short port);
	bool acceptClient();
	bool sendMsg(const char* msg);
	bool recvMsg();
	const char* getRecvBuf();
private:
	SOCKET serverSock;
	SOCKET clientSock;
	//要加{}初始化，否则会被随机填充
	char recvBuf[BufSize]{};
	bool init(const char* ip, unsigned short port);
};

#endif