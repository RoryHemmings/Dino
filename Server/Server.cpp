#include <iomanip>

#include "Server.h"

using namespace std;

Server::Server(int port) 
	: socket(port), clients(), current() 
{ }

void Server::listConnections() const
{
	if (clients.size() < 1) { cout << "There are no current connections" << endl; }
	for (vector<Client>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
		string active = (it->socket == current.socket) ? "current" : "";
		cout << it->ip << setw(10) << active << endl;
	}
}
