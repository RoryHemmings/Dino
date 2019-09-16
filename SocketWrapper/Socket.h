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
	// recieves until newline
	std::string recieveLine() const;
	std::string recieveBytes() const;

	// sends single line
	void sendLine(std::string) const;
	void sendBytes(const std::string&) const;

	void close() { closesocket(s); };

protected:
	friend class ServerSocket;
	friend class ClientSocket;

	Socket(SOCKET pSocket) : s(pSocket) { start(); }
	Socket();

	SOCKET s;

private:
	void start();
	void stop();

};

class ServerSocket : public Socket
{

public:
	// creates socket, binds it, and listens
	ServerSocket(int port, int backlog=10);

	Socket* accept();
	
};

class ClientSocket : public Socket
{

public:
	ClientSocket(const std::string& host, int port);
};

#endif

