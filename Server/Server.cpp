#include <iomanip>

#include "Server.h"

using namespace std;

Client& Client::operator=(const Client& rhs)
{
	if (&rhs != this) {
		socket = (rhs.socket) ? rhs.socket->clone() : 0;
		return *this;
	}
}

Server::Server(int port) 
	: master_socket(port, true), iPort(port), current(), running(true), recieve_thread(&Server::r_loop, this)
{
}

void Server::stop()
{
	running = false;
	recieve_thread.join();

	master_socket.close();
	for (vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
		it->close();
	}
}

void Server::sendCurrent(const Command& command, const vector<string>& args) const
{
	switch (command)
	{
		case RUN:
			string param = "";
			for (vector<string>::size_type i = 0; i < args.size(); ++i) {
				param += args[i] + " ";
			}

			current->sendLine("run;" + param);
			break;
	}
}

void Server::listConnections() const
{
	if (clients.size() < 1) { cout << "There are no current connections" << endl; }
	for (vector<Client>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
		string active = (*it == current) ? "current" : "";
		cout << (*it)->getAddr() << setw(10) << active << endl;
	}
}

bool Server::setCurrent(const string& ip)
{
	std::vector<Client>::iterator iter = std::find(clients.begin(), clients.end(), ip);
	if (iter == clients.end()) return false;

	current = *iter;
	return true;
}

string Server::currentIp() const
{
	if (current)
		return current->getAddr();
	else
		return "";
}

void Server::r_loop()
{
	Socket* s;
	int max_sd, sd, activity;
	fd_set readfds;

	while (running) {
		// Clear socket set
		FD_ZERO(&readfds);

		// Add master socket to socket set (by descriptor)
		FD_SET(master_socket.dsc(), &readfds);
		max_sd = master_socket.dsc();

		// Add all clients to socket set (by descriptor)
		for (int i = 0; i != clients.size(); ++i) {
			// Get descriptor
			sd = clients[i]->dsc();

			// Add to socket set
			if (sd > 0)
				FD_SET(sd, &readfds);

			max_sd = max(sd, max_sd);
		}

		// Wait for activity to occur
		activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

		if (activity < 0) cout << "select error" << endl;

		// Incoming connection
		if (FD_ISSET(master_socket.dsc(), &readfds)) {
			// Accept and add to client vector
			s = master_socket.accept();
			if (s != 0) {
				Client temp(s);
				clients.push_back(temp);
				cout << "\nNew Client accepted: " << temp->getAddr() << endl << "> " << currentIp();
				temp->sendLine("welcome");
			}
		}

		// Check for client IO operations
		for (int i = 0; i < clients.size(); ++i) {
			sd = clients[i]->dsc();

			if (FD_ISSET(sd, &readfds)) {
				string r = clients[i]->recieveBytes();
				if (r == "!disconnected" || r == "!error") {
					LOGGER.log(clients[i]->getAddr() + " has disconnected", "", SECONDARY);
					clients.erase(clients.begin() + i);
				}
				else {
					LOGGER.log(r, clients[i]->getAddr(), SUCCESS);
				}
			}
		}
	}
}
