#pragma once

#include "util.h"

// 疑惑 recv 和 send 可以在不同作用域中吗

class CServerBroadcast {

	char*	MsgBuf;
	int		sz_MsgBuf;
	int		iResult;
	WSADATA	wsaData;
	SOCKET	sourceClient;
	std::pair<SOCKET, std::string>	box;
	char*	IPstring;
	std::map<SOCKET, sockaddr_in>::iterator it;

public:

	CServerBroadcast();
	~CServerBroadcast();

	void switchOn();

private:

	bool getMsg();

};