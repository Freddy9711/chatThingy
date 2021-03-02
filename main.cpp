#include "LogicCore.h"
#include "ServerBroadcast.h"
#include "ServerListener.h"
#include "util.h"

void t_Listener(int port = 1101) {
	CServerListener Listener(port);
	Listener.switchOn();
}

void t_Core() {
	CLogicCore Core;
	Core.switchOn();
}

void t_Broadcast() {
	CServerBroadcast Broadcast;
	Broadcast.switchOn();
}

int main() {


	int port = 1101;
	std::thread t1(t_Listener, port);
	std::thread t2(t_Core);
	std::thread t3(t_Broadcast);

	t1.join();
	t2.join();
	t3.join();
}