#pragma once

#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <string>
#include <iostream>

#include <SocketWrapper.h>

#include <windows.h>
#include <stdio.h>

struct Client {
	Client(Socket* s) : socket(s), ip("0.0.0.0") { }
	Client() : socket(NULL), ip("0.0.0.0") { }
	~Client() { delete socket; }

	Socket* socket;
	std::string ip;
};

class Server
{

public:
	Server(int port);
	Server() : Server(DEFAULT_PORT) { }

	// TODO add rule of three

	// Waits for clients
	void serve();

	// Talks with current connected client
	void talk();

	void setCurrent(Socket* c) { current = c; }
	void listConnections() const;

	void stop() { socket.close(); }

	int port() const { return iPort; }

private:
	ServerSocket socket;
	std::vector<Client> clients;

	Client current;

	int iPort;
};

#endif

