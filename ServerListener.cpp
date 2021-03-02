#include "ServerListener.h"


CServerListener::CServerListener(int port) {
;
	printf("Initializing Server Listener ...\n");

	wsaData						= { 0 };
	iResult						= 0;
	
	Listener					= INVALID_SOCKET;
	Client						= INVALID_SOCKET;

	LAddr.sin_family			= AF_INET;
	LAddr.sin_addr.S_un.S_addr	= INADDR_ANY;
	this->port					= port;
	LAddr.sin_port				= htons(port);

	FD_ZERO(&fdSlct);

	box							= {INVALID_SOCKET, ""};

	sz_recv						= 0;
	//sz_recvMax				= sizeof(recvBuf);
	//sz_MsgBuf					= sizeof(MsgBuf);
	sz_recvMax					= 256;
	sz_MsgBuf					= 64 * 1024;

	//recvBuf = (char*)malloc(sz_recvMax);
	recvBuf						= new char[sz_recvMax];
	memset(recvBuf, 0, sz_recvMax);
	MsgBuf						= new char[sz_MsgBuf];
	memset(MsgBuf, 0, sz_MsgBuf);
	IPstring					= new char[20];
	memset(IPstring, 0, 20);

	writeHere					= 0;

	on							= false;
	off							= true;

	printf("Server Listener initialized!\n");
}

CServerListener::~CServerListener() {

	printf("Destructing Server Listener ...\n");
	// �رտ����������е�ѭ��
	on = false;
	int count = 0;
	while (!on) {
		if (off) {
			// ���������Ѿ�����ѭ��
			printf("Server Listener destructed!\n");
			break;
		}
		else if (count == 5) {
			closesocket(Listener);
			WSACleanup();
			off = true;
		}
		else {
			Sleep(5000);
			++count;
		}
	}
}

int CServerListener::switchOn() {

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return -1;
	}

	Listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Listener == INVALID_SOCKET) {
		printf("socket() failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	if (bind(Listener, (sockaddr*)&LAddr, sizeof(LAddr)) == SOCKET_ERROR) {
		printf("bind() failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	if (listen(Listener, 10) == SOCKET_ERROR) {
		printf("listen() failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	FD_SET(Listener, &fdSlct);
	timeval T = { 0, 0 };

	// ���� select ѭ����
	on = true;

	while (on) {

		// ÿ�����½�������֪�׽�������fdRead�Լ��ɶ�״̬
		fd_set fdRead = fdSlct;
		iResult = select(0, &fdRead, NULL, NULL, &T);
		// select()�Ὣ���ɶ����׽������fdRead
		if (iResult == SOCKET_ERROR) {
			printf("select() failed with error: %d\n", WSAGetLastError());
			WSACleanup();
			return -1;
		}
		else if (iResult == 0) {
			// ��δ��������
			continue;
		}
		else {
			// ��ѯ��ǰ�����׽���
			for (int i = 0; i < (int)fdSlct.fd_count; i++) {
				// �Ƿ�ɶ�
				if (FD_ISSET(fdSlct.fd_array[i], &fdRead)) {
					if (fdSlct.fd_array[i] == Listener) {
						// ��ǰ�ɶ��׽���Ϊ�����������׽���
						if (fdSlct.fd_count == FD_SETSIZE) {
							printf("select fd_set is full!\n");
							continue;
						}
						else {
							// Ϊ���������󴴽��׽���
							sockaddr_in remoteAddr;
							int			remoteAddrLen = sizeof(remoteAddr);
							SOCKET client = accept(Listener, (sockaddr*)&remoteAddr, &remoteAddrLen);
							if (client == INVALID_SOCKET) {
								printf("accept() failed with error: %d\n", WSAGetLastError());
								WSACleanup();
								return -1;
							}
							else {
								inet_ntop(AF_INET, & remoteAddr.sin_addr, IPstring, 20);
								printf("%s has connected! \n", IPstring);
								FD_SET(client, &fdSlct);
								record(client, remoteAddr);
							}
						}
					}
					else {
						// ��ǰ�ɶ��׽���Ϊ�ͻ���ͨ���׽���
						// ������ȡ���û����͵Ķ�
						Client = fdSlct.fd_array[i];
						while(true) {
							sz_recv = recv(Client, recvBuf, sz_recvMax, 0);
							if (sz_recv > 0) {
								// �յ��ͻ����͵Ķ�
								bufCurator();
							}
							else {
								// δ�յ���
								break;
							}
						}
						if (sz_recv == SOCKET_ERROR) {
							// �û�������
							printf("recv() failed with error: %d\n", WSAGetLastError());
							FD_CLR(Client, &fdSlct);
							erase(Client);
							closesocket(Client);
							continue;
						}
						else {
							// �ͻ����ŵعر�������
							// �ͻ����ܻ����Խ�����Ϣ
							FD_CLR(Client, &fdSlct);
						}
					}
				}
				else {
					// �����ڴ�����߳���ʱ������Ӧ�ͻ�
					continue;
				}
			}
		}

	}
	
	// �뿪 select ѭ����
	off = true;

	return 0;

}

int CServerListener::switchOff() {

	on = false;
	Sleep(5000);
	if (off) {
		shutdown(Listener, SD_BOTH);
		//ClntInfo.clear();
		WSACleanup();
		return 0;
	}
	else {
		return -1;
	}
}

void CServerListener::record(SOCKET socket, sockaddr_in addr) {

	std::unique_lock<std::mutex> lk_LiBr(mtx_LiBr);
#if 0
	while (ClntInfo.size() == sz_ClntInfo) {
		printf("Maximum client number has been reached. Suspending Server Listener...\n");
		cv_LiBr_not_full.wait(lk_LiBr);
	}
	ClntInfo[socket] = addr;
#endif
#if 1
	while (cInfo.isFull()) {
		printf("Maximum client number has been reached. Suspending Server Listener...\n");
		cv_LiBr_not_full.wait(lk_LiBr);
	}
	cInfo.Put(socket, addr);
#endif
	cv_LiBr_not_empty.notify_all();				// ֪ͨ Server Broadcast �пͻ���Ϣ��д��
	inet_ntop(AF_INET, &addr.sin_addr, IPstring, 20);
	printf("%s has been added to the client list by Server Listener! \n", IPstring);

}

void  CServerListener::erase(SOCKET socket) {

	std::unique_lock<std::mutex> lk_LiBr(mtx_LiBr);
#if 0
	if (ClntInfo.empty()) return;
	IN_ADDR temp = ClntInfo[socket].sin_addr;
	ClntInfo.erase(socket);
#endif
#if 1
	if (cInfo.isEmpty()) return;
	sockaddr_in temp;
	cInfo.Get(socket, temp);
	cInfo.Toss(socket);
#endif
	inet_ntop(AF_INET, &(temp.sin_addr), IPstring, 20);
	printf("%s has been removed from the client list by Server Listener! \n", IPstring);

}

void CServerListener::bufCurator() {

	if (sz_recv >= (int)strlen(CHead)) {
		char temp[9] = { 0 };
		memmove(temp, recvBuf, strlen(CHead));
		if (strcmp(temp, CHead) == 0 && writeHere != 0) {
			std::string Msg = (std::string)MsgBuf;
			std::unique_lock<std::mutex> lk_LiLo(mtx_LiLo);
			while (MsgQ_LiLo.isFull()) {
				printf("Receive message list is full. Suspending Server Listner...\n");
				cv_LiLo_not_full.wait(lk_LiLo);
			}
			box = { Client, Msg };
			MsgQ_LiLo.Put(box);
			box = { INVALID_SOCKET, "" };
			cv_LiLo_not_empty.notify_all();			// ֪ͨ LogicCore ����Ϣ������

			writeHere = 0;						// дָ���λ
		}
	}
	// ����������������ν�����Ϣ����С�ڱ�ͷ���ȣ����ν�����Ϣ���ޱ�ͷ
	memmove(MsgBuf + writeHere, recvBuf, sz_recv);
	writeHere += sz_recv - 1;

}