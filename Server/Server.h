#pragma once

#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <thread>

#include <SocketWrapper.h>
#include <Logger.h>

#include <windows.h>
#include <stdio.h>

enum Command {
	RUN
};

class Client {

public:
	// Socket Handle (Blocking by default)
	Client() : socket(0) { }																// Default constructor
	Client(Socket* s) : socket(s) { }														// Conversion consturctor from Socket* to Client object
	Client(const Client& rhs) : socket() { if (rhs.socket) socket = rhs.socket->clone(); }	// Copy constructor
	Client& operator=(const Client& rhs);													// Assignment operator
	~Client() { delete socket; }															// Destructor

	bool operator==(const std::string& rhs) const { return socket->getAddr() == rhs; }		// Comparison operator
	void close() { socket->close(); }														// Close socket							

	operator bool() const { return socket; }												// Bool conversion
	Socket& operator*() const { if (socket) return *socket; }								// Dereference operator
	Socket* operator->() const { if (socket) return socket; }								// Arrow operator
	

private:
	Socket* socket;

};

class Server
{

public:
	explicit Server(int port);
	Server() : Server(DEFAULT_PORT) { }

	void sendCurrent(const Command& command, const std::vector<std::string>& args) const;

	bool setCurrent(const std::string& ip);
	void listConnections() const;

	void stop();

	int port() const { return iPort; }
	std::string currentIp() const;

private:
	ServerSocket master_socket;
	std::vector<Client> clients;

	Client current;

	int iPort;
	bool running;

	std::thread recieve_thread;
	void r_loop();
};

#endif

//struct Client {
//	// Creates blocking socket (exits when recieves the exit command)
//	Client(Socket* s, std::string addr) : socket(s), ip(addr), recv_loop(&Client::r_loop, this) { }
//	Client() : socket(NULL), ip("0.0.0.0") { }
//
//	Client& operator=(const Client& rhs) {
//		if (&rhs != this) {
//			socket = rhs.socket;
//			ip = rhs.ip;
//
//			return *this;
//		}
//	}
//	~Client() { recv_loop.join(); socket = 0; }
//
//	bool operator==(const std::string& rhs) const { return ip == rhs; }
//	void close() { socket->close(); }
//
//	operator bool() const { return socket; }
//
//	void r_loop();
//
//	Socket* socket;
//	std::string ip;
//
//	std::thread recv_loop;
//};

