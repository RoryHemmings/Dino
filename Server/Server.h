#pragma once

#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <thread>

#include <SocketWrapper.h>

#include <windows.h>
#include <stdio.h>

enum commands {

};

struct Client {
	Client(Socket* s, std::string addr) : socket(s), ip(addr) { }
	Client() : socket(NULL), ip("0.0.0.0") { }

	Client& operator=(const Client& rhs) { 
		if (&rhs != this) { 
			socket = rhs.socket; 
			ip = rhs.ip;

			return *this; 
		}
	}
	~Client() { socket = 0; }

	bool operator==(const std::string& rhs) const { return ip == rhs; }
	void close() { socket->close(); }
	 
	Socket* socket;
	std::string ip;
};

class Server
{

public:
	explicit Server(int port);
	Server() : Server(DEFAULT_PORT) { }

	// TODO add rule of three

	int setCurrent(const std::string& ip);
	void listConnections() const;

	void stop();

	int port() const { return iPort; }

private:
	ServerSocket socket;
	std::vector<Client> clients;

	Client current;

	int iPort;
	bool running;

	std::vector<std::thread> connections;
	std::thread acceptLoop;

	// Waits for clients
	void accept();
};

#endif

