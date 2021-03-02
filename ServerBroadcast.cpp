#include "ServerBroadcast.h"
#include <WS2tcpip.h>

CServerBroadcast::CServerBroadcast()
{
	printf("Initiating Server Broadcast...\n");
	sz_MsgBuf = 64 * 1024;
	MsgBuf = new char[sz_MsgBuf];
	memset(MsgBuf, 0, sz_MsgBuf);
	IPstring = new char[20];
	memset(IPstring, 0, 20);
	box = { INVALID_SOCKET, "" };
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	sourceClient = INVALID_SOCKET;
	if (iResult == SOCKET_ERROR) {
		printf("Server Broadcast initiation failed with error: %d\n", WSAGetLastError());
		WSACleanup();
	}
	else {
		iResult = 0;
		printf("Server Broadcast has been initated!\n");
	}

}

CServerBroadcast::~CServerBroadcast()
{
	closesocket(sourceClient);
	WSACleanup();
	printf("Server Broadcast is destroyed!\n");
}

void CServerBroadcast::switchOn()
{
#if 0
	while (true) {
		if (getMsg() == false) continue;

		std::unique_lock<std::mutex> lk_BrLi(mtx_LiBr);
		while (ClntInfo.empty()) {
			printf("Server Broadcast waiting for connection...\n");
			cv_LiBr_not_empty.wait(lk_BrLi);
		}
		for (std::map<SOCKET, sockaddr_in>::iterator it = ClntInfo.begin(); it != ClntInfo.end(); ++it) {
			//if (it->first == sourceClient) continue;
			iResult = send(it->first, MsgBuf, strlen(MsgBuf), 0);
			if (iResult == SOCKET_ERROR) {
				inet_ntop(AF_INET, &it->second.sin_addr, IPstring, 20);
				printf("%s has been removed from the client list by Server Broadcast! \n", IPstring);
				shutdown(it->first, SD_BOTH);
				ClntInfo.erase(it->first);
				cv_LiBr_not_full.notify_all();
			}
		}
	}
#endif
#if 1
	while (true) {
		if (getMsg()) {
			std::unique_lock<std::mutex> lk_BrLi(mtx_LiBr);
			while (cInfo.isEmpty()) {
				printf("Server Broadcast waiting for connection...\n");
				cv_LiBr_not_empty.wait(lk_BrLi);
			}
			while (cInfo.Iter(it)) {
				iResult = send(it->first, MsgBuf, strlen(MsgBuf), 0);
				if (iResult == SOCKET_ERROR) {
					SOCKET sockt = it->first;
					IN_ADDR adt = it->second.sin_addr;
					inet_ntop(AF_INET, &adt, IPstring, 20);
					shutdown(sockt, SD_BOTH);
					cInfo.Toss(sockt, it);
					printf("%s has been removed from the client list by Server Broadcast! \n", IPstring);
					cv_LiBr_not_full.notify_all();
				}
			}
		}
		else {
			continue;
		}
	}
#endif
}

bool CServerBroadcast::getMsg()
{
	std::unique_lock<std::mutex> lk_BrLo(mtx_LoBr);
	while (MsgQ_LoBr.isEmpty()) {
		printf("Server Broadcast waiting for message to send...\n");
		cv_LoBr_not_empty.wait(lk_BrLo);
	}
	MsgQ_LoBr.Get(box);
	cv_LiBr_not_full.notify_all();

	u_int headStart = std::string::npos;
	headStart = box.second.find(SHead);
	if (headStart == std::string::npos) {
		printf("Server Broadcast discarded an invalid message! \n");
		box = { INVALID_SOCKET, "" };
		return false;
	}
	box.second.erase(0, headStart);
	sourceClient = box.first;
	strcpy_s(MsgBuf, sz_MsgBuf, box.second.c_str());
	box = { INVALID_SOCKET, "" };

	return true;
}
