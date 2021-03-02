#pragma once
#include <WinSock2.h>
#include <map>

class CClientInfo {
	std::map<SOCKET, sockaddr_in> map;
	size_t sz;
	bool in_iter;
public:
	CClientInfo(u_int sz = 256);
	bool Get(SOCKET& s, sockaddr_in& addr);
	bool Put(SOCKET& s, sockaddr_in& addr);
	void Toss(SOCKET& s, std::map<SOCKET, sockaddr_in>::iterator& it);
	void Toss(SOCKET& s);
	bool Iter(std::map<SOCKET, sockaddr_in>::iterator& it);
	bool isFull();
	bool isEmpty();
};