#include <iomanip>

#include "Server.h"

using namespace std;

Server::Server(int port) 
	: socket(port), iPort(port), current(), running(true), acceptLoop(&Server::accept, this)
{
}

void Server::stop()
{
	running = false;
	acceptLoop.join();

	socket.close();
	for (vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
		it->close();
	}
}

void Server::listConnections() const
{
	if (clients.size() < 1) { cout << "There are no current connections" << endl; }
	for (vector<Client>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
		string active = (it->socket == current.socket) ? "current" : "";
		cout << it->ip << setw(10) << active << endl;
	}
}

int Server::setCurrent(const string& ip)
{
	std::vector<Client>::const_iterator iter = std::find(clients.begin(), clients.end(), ip);
	if (iter == clients.end()) return 0;

	current = *iter;
	return 1;
}

void Server::accept()
{
	Socket* s;
	
	while (running) {
		s = socket.accept();
		if (s != 0) {
			Client temp(s, s->getAddr());
			clients.push_back(temp);
		}
	}
}
