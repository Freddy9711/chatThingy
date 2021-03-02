#pragma once

#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
#include <string>
#include <WinSock2.h>
#include <map>
#include <ws2tcpip.h>
#include <stdio.h>

#include "ClientInfo.h"
#include "MsgQueue.h"

// ������Ϣͷ
__declspec(selectany) char CHead[9] = "$$WSAD$$";

// ������Ϣͷ
__declspec(selectany)char SHead[9] = "$$QWER$$";

// �̻߳���
__declspec(selectany) std::mutex mtx_LiLo;
__declspec(selectany) std::mutex mtx_LoBr;
__declspec(selectany) std::mutex mtx_LiBr;

// �������ͷ�����
__declspec(selectany) std::condition_variable cv_LiLo_not_empty;
__declspec(selectany) std::condition_variable cv_LiLo_not_full;
__declspec(selectany) std::condition_variable cv_LoBr_not_empty;
__declspec(selectany) std::condition_variable cv_LoBr_not_full;
__declspec(selectany) std::condition_variable cv_LiBr_not_empty;
__declspec(selectany) std::condition_variable cv_LiBr_not_full;

// ��Ʒ��
__declspec(selectany) CMsgQ MsgQ_LiLo;
__declspec(selectany) CMsgQ MsgQ_LoBr;
__declspec(selectany) CClientInfo cInfo;

#if 0
// ȫ����Ϣ����Ϣ��������Ʒ�⣩
extern std::queue<std::pair<SOCKET, std::string>> MsgQ_LiLo;
extern std::queue<std::pair<SOCKET, std::string>> MsgQ_LoBr;
extern std::map<SOCKET, sockaddr_in> ClntInfo;

extern size_t sz_MsgQ_LiLo;
extern size_t sz_MsgQ_LoBr;
extern size_t sz_ClntInfo;
#endif

#if 0
// ����װ������

class MsgQ {
	std::queue<std::pair<SOCKET, std::string>> q;
	size_t sz;
public:
	MsgQ(u_int sz = 256);
	bool Get(std::pair<SOCKET, std::string>& box);
	bool Put(std::pair<SOCKET, std::string>& box);
};

class ClntInfo {
	std::map<SOCKET, sockaddr_in> map;
	size_t sz;
public:
	ClntInfo(u_int sz = 256);
	bool Get(SOCKET& s, sockaddr_in& addr);
	bool Put(SOCKET& s, sockaddr_in& addr);
	void Toss(SOCKET& s);
private:
	bool isFull();
};

#endif