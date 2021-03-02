#pragma once
#include <queue>
#include <WinSock2.h>
#include <string>

class CMsgQ {
	std::queue<std::pair<SOCKET, std::string>> q;
	size_t sz;
public:
	CMsgQ(u_int sz = 256);
	bool Get(std::pair<SOCKET, std::string>& box);
	bool Put(std::pair<SOCKET, std::string>& box);
	bool isEmpty();
	bool isFull();
};