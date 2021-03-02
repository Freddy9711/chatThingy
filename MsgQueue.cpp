#include "MsgQueue.h"

CMsgQ::CMsgQ(u_int sz) {
	q = {};
	this->sz = sz;
}

bool CMsgQ::Get(std::pair<SOCKET, std::string>& box) {
	if (isEmpty()) return false;
	box = q.front();
	q.pop();
	return true;
}

bool CMsgQ::Put(std::pair<SOCKET, std::string>& box) {
	if (isFull()) return false;
	q.push(box);
	return true;
}

bool CMsgQ::isFull() {
	if (q.size() == sz) return true;
	else return false;
}

bool CMsgQ::isEmpty() {
	if (q.empty()) return true;
	else return false;
}