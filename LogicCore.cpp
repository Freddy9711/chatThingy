#include "LogicCore.h"

CLogicCore::CLogicCore() {
	
	printf("Initiating Logic Core! \n");
	sz_MsgBuf = 64 * 1024;
	MsgBuf.reserve(sz_MsgBuf);
	box = { INVALID_SOCKET, "" };
	invalidMsg = false;
	printf("Logic Core is initiated! \n");

}

CLogicCore::~CLogicCore() {
	printf("Logic Core is terminated! \n");
}

void CLogicCore::switchOn()
{
	while (true) {
		getMsg();
		cookMsg();
		sendMsg();
	}
}

void CLogicCore::cookMsg()
{
	chopHead();
	pasteHead();
}

void CLogicCore::getMsg()
{
	std::unique_lock<std::mutex> lk_LoLi(mtx_LiLo);
	while (MsgQ_LiLo.isEmpty()) {
		printf("Waiting for messages... Logic Core suspended.\n");
		cv_LiLo_not_empty.wait(lk_LoLi);			// 等待 Server Listener 通知消息队列已有产品
	}
	MsgQ_LiLo.Get(box);
	cv_LiLo_not_full.notify_all();					// 通知 Server Listener 消息队列已有空位
	MsgBuf = box.second;
	box.second = "";
}

void CLogicCore::sendMsg()
{
	if (invalidMsg) {
		box = { INVALID_SOCKET, "" };
		return;
	}
	box.second = MsgBuf;
	std::unique_lock<std::mutex> lk_LoBr(mtx_LoBr);
	while (MsgQ_LoBr.isFull()) {
		printf("Waiting for empty slot to pass messages... Logic Core suspended.\n");
		cv_LoBr_not_full.wait(lk_LoBr);				// 等待 Server Broadcast 通知消息队列已有空位
	}
	MsgQ_LoBr.Put(box);
	cv_LoBr_not_empty.notify_all();					// 通知 Server Broadcast 消息队列已有产品
	box = { INVALID_SOCKET, "" };
}

void CLogicCore::chopHead()
{
	u_int headStart = std::string::npos;
	headStart = MsgBuf.find(CHead);
	if (headStart == std::string::npos) {
		printf("Warning: An invalid message is received! \n");
		invalidMsg = true;
		box = { INVALID_SOCKET, "" };
		return;
	}
	MsgBuf.erase(0, headStart + strlen(CHead));
}

void CLogicCore::pasteHead()
{
	if (invalidMsg) {
		box = { INVALID_SOCKET, "" };
		return;
	}
	MsgBuf.insert(0, SHead);
}
