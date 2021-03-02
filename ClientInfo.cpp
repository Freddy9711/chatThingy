#include "ClientInfo.h"

CClientInfo::CClientInfo(u_int sz) {
	map = {};
	this->sz = sz;
	in_iter = false;
}

bool CClientInfo::Get(SOCKET& s, sockaddr_in& addr) {
	if (isEmpty()) return false;
	addr = map[s];
	return true;
}

bool CClientInfo::Put(SOCKET& s, sockaddr_in& addr) {
	if (isFull()) return false;
	map[s] = addr;
	return true;
}

void CClientInfo::Toss(SOCKET& s, std::map<SOCKET, sockaddr_in>::iterator& it) {
	// �ڵ�����ɾ��Ԫ��ʱҪС�ĵ�����ʧЧ
	if (it == map.begin()) {
		in_iter = false;
	}
	else {
		--it;
	}
	map.erase(s);
}

void CClientInfo::Toss(SOCKET& s) {
	map.erase(s);
}

bool CClientInfo::Iter(std::map<SOCKET, sockaddr_in>::iterator& it)
{
	if (in_iter) {
		if (++it == map.end()) {
			in_iter = false;
			// �˳�����
		}
	}
	else {
		// �������
		it = map.begin();
		in_iter = true;
	}
	return in_iter;
}

bool CClientInfo::isFull() {
	if (map.size() == sz) return true;
	else return false;
}

bool CClientInfo::isEmpty() {
	if (map.empty()) return true;
	else return false;
}