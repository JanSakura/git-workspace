#ifndef _CLIENT_
#define _CLIENT_
#include<WinSock2.h>
#define BufSize 128

class Client {
public:
	Client();
	virtual ~Client();
	bool start(const char* ip, unsigned short port);
	bool sendMsg();
	bool recvMsg();
	void close();

private:
	SOCKET servSock;
	char recvBuf[BufSize]{};
	//recv·µ»ØµÄ³¤¶È
	int recvRet = 0;
	bool init(const char* ip, unsigned short port);
};

#endif // !_CLIENT_

