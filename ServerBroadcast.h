#pragma once

#include "util.h"

// �ɻ� recv �� send �����ڲ�ͬ����������

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