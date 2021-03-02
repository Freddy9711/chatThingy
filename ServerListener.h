#pragma once

#include <queue>
#include <map>
#include <string>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "util.h"


class CServerListener {

private:

	// �׽��ּ����
	WSADATA				wsaData;
	int					iResult;
	SOCKET				Listener;
	SOCKET				Client;
	sockaddr_in			LAddr;
	int					port;
	char*				IPstring;

	fd_set				fdSlct;

	std::pair<SOCKET, std::string> box;

	// ���ô�СBuf��ʵ����ȡ���飬��СBuf���жϱ�ͷ���ڴ�Buf���ۻ�һ����ͷ�μ�������Ǳ�ͷ��
	//char				MsgBuf[64 * 1024] = { 0 };
	//char				recvBuf[256] = { 0 };
	char*				MsgBuf;
	char*				recvBuf;
	int 				writeHere;

	int					sz_recv;
	int					sz_recvMax;
	int					sz_MsgBuf;

	// selectѭ������

	bool				on;
	bool				off;
	/************************************************/
	/* on == true  && off == false : ����ѭ������   */
	/* on == false && off == false  :���ڽ���ѭ���� */
	/* on == false && off == true  : ����ѭ������   */
	/************************************************/
	

public:

	CServerListener(int port = 1101);
	~CServerListener();

	// ����ѭ������
	int switchOn();
	int switchOff();

private:

	// �ͻ���Ϣ�Ǽ�
	void record(SOCKET socket, sockaddr_in addr);

	// �ͻ���Ϣ����
	void erase(SOCKET socket);

	// ��Ϣ����
	void bufCurator();

};
