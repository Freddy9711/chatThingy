#pragma once

#include <string>
#include <queue>
#include <WinSock2.h>
#include "util.h"


class CLogicCore {

private:

	std::string						MsgBuf;
	int								sz_MsgBuf;
	std::pair<SOCKET, std::string>	box;
	bool							invalidMsg;

public:

	CLogicCore();
	~CLogicCore();
	
	void switchOn();

private:

	void cookMsg();

	void getMsg();
	void sendMsg();

	void chopHead();
	void pasteHead();
	
	// more to come...
	
};