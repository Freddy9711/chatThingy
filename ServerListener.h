#pragma once

#include <queue>
#include <map>
#include <string>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "util.h"


class CServerListener {

private:

	// 套接字及相关
	WSADATA				wsaData;
	int					iResult;
	SOCKET				Listener;
	SOCKET				Client;
	sockaddr_in			LAddr;
	int					port;
	char*				IPstring;

	fd_set				fdSlct;

	std::pair<SOCKET, std::string> box;

	// 采用大小Buf来实现收取分组，在小Buf中判断标头，在大Buf中累积一个标头段及其后续非标头段
	//char				MsgBuf[64 * 1024] = { 0 };
	//char				recvBuf[256] = { 0 };
	char*				MsgBuf;
	char*				recvBuf;
	int 				writeHere;

	int					sz_recv;
	int					sz_recvMax;
	int					sz_MsgBuf;

	// select循环控制

	bool				on;
	bool				off;
	/************************************************/
	/* on == true  && off == false : 处于循环体中   */
	/* on == false && off == false  :正在结束循环体 */
	/* on == false && off == true  : 不在循环体中   */
	/************************************************/
	

public:

	CServerListener(int port = 1101);
	~CServerListener();

	// 监听循环开关
	int switchOn();
	int switchOff();

private:

	// 客户信息登记
	void record(SOCKET socket, sockaddr_in addr);

	// 客户信息擦除
	void erase(SOCKET socket);

	// 消息处理
	void bufCurator();

};
