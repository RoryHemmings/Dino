//#pragma once
//
//#ifndef SERVER_H
//#define SERVER_H
//
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <iphlpapi.h>
//
//#include <windows.h>
//#include <stdio.h>
//
//class Server
//{
//
//public:
//	Server() : Server(27015) { }
//	Server(int port);
//
//	SOCKET getSocket() { return listenSocket; }
//	const SOCKET getSocket() const { return listenSocket; }
//
//	int accept();
//	int recieve();
//	int close();
//
//private:
//	SOCKET listenSocket, clientSocket;
//	int port, error;
//
//};
//
//#endif
//
