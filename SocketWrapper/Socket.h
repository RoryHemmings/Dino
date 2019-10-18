#pragma once

#ifndef SOCKET_H
#define SOCKET_H

#include <stdio.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <string>

#include <Windows.h>

#pragma comment(lib, "Ws2_32.lib")

class Socket
{

public:
	// Recieves until newline (\n)
	std::string recieveLine() const;

	// Recieves string until stream runs out of bytes
	std::string recieveBytes() const;

	// Sends single line (ends with newline, \n)
	void sendLine(std::string) const;

	// Sends string as byte array
	void sendBytes(const std::string&) const;

	// Close SOCKET
	void close() { closesocket(s); }

	// Get ip adress used by the SOCKET
	std::string getAddr() const;

	// Return descriptor used by socket
	int dsc() const { return s; }

	// Return copy of object (in the future I might add polymorphic copying)
	virtual Socket* clone() const { return new Socket(*this); }

protected:
	friend class ServerSocket;
	friend class ClientSocket;

	// Conversion constructor from SOCKET to Socket
	Socket(SOCKET pSocket, bool nonBlocking=false) : s(pSocket), nonBlocking(nonBlocking) { start(); }

	// Default constructor (makes empty socket)
	Socket(bool nonBlocking=false);

	// Internal SOCKET object
	SOCKET s;

	// Wether or not the socket should block
	bool nonBlocking;

private:
	// Startup winsock
	void start();

	// Cleanup winsock
	void stop();

};

class ServerSocket : public Socket
{

public:
	// Creates a server socket, binds it, and listens
	explicit ServerSocket(int port, bool nonBlocking=false, int backlog=10);

	// Accepts new connection and returns Socket* to the object (either blocking or non-blocking depending on socket type)
	Socket* accept();
	
};

class ClientSocket : public Socket
{

public:
	// Creates a client socket, attepmts to connect to host on port
	explicit ClientSocket(const std::string& host, int port, bool nonBlocking=false);
};

#endif


