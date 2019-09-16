//#include "Server.h"
//
//using namespace std;
//
//Server::Server(int port) : port(port), error(0), listenSocket(INVALID_SOCKET),
//		clientSocket(INVALID_SOCKET)
//{
//	WSADATA wsaData;
//
//	// Initialize Winsock
//	int r;
//	r = WSAStartup(MAKEWORD(2, 2), &wsaData);
//	if (r) {
//		printf("WSAStartup failed: %d\n", r);
//		error = 1;
//		return;
//	}
//
//	struct addrinfo* result = NULL, * ptr = NULL, hints;
//
//	// Set Adress Settings
//	ZeroMemory(&hints, sizeof(hints));
//	hints.ai_family = AF_INET;
//	hints.ai_socktype = SOCK_STREAM;
//	hints.ai_protocol = IPPROTO_TCP;
//	hints.ai_flags = AI_PASSIVE;
//
//	// Resolve the local address and port to be used by the server, Take data from 'hints' struct and put it into 'result' struct
//	// Replace Null with hostname (if desired to be something other than localhost)
//	r = getaddrinfo(NULL, (PCSTR) port, &hints, &result);
//	if (r) {
//		printf("getaddrinfo failed: %d\n", r);
//		WSACleanup();
//		error = 1;
//		return;
//	}
//
//	// Initialize Socket
//	SOCKET listenSocket = INVALID_SOCKET;
//	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
//
//	if (listenSocket == INVALID_SOCKET) {
//		printf("Error at socket(): %ld\n", WSAGetLastError());
//		freeaddrinfo(result);
//		WSACleanup();
//		error = 1;
//		return;
//	}
//
//	// Bind Socket
//	r = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
//	if (r == SOCKET_ERROR) {
//		printf("bind failed with error: %d\n", WSAGetLastError());
//		freeaddrinfo(result);
//		closesocket(listenSocket);
//		WSACleanup();
//		error = 1;
//		return;
//	}
//
//	// Free space allocated to the Adress Info
//	freeaddrinfo(result);
//
//	// Listen on socket
//	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
//		printf("Listen failed with error: %ld\n", WSAGetLastError());
//		closesocket(listenSocket);
//		WSACleanup();
//		error = 1;
//		return;
//	}
//
//}
//
//int Server::accept()
//{
//	clientSocket = ::accept(listenSocket, NULL, NULL);
//	if (clientSocket == INVALID_SOCKET) {
//		printf("accept failed: %d\n", WSAGetLastError());
//		closesocket(listenSocket);
//		WSACleanup();
//		return 1;
//	}
//}
//
//int Server::recieve() {
//	int r, rs;
//
//	char recvbuf[512];
//	int recvbuflen = 512;
//
//	// Receive bytes
//	do {
//		// Returns length of recvbuffer
//		r = recv(clientSocket, recvbuf, recvbuflen, 0);
//		if (r > 0) {
//			printf("Bytes received: %d\n", r);
//
//			// Send the buffer back to the sender
//			rs = send(clientSocket, recvbuf, r, 0);
//			if (rs == SOCKET_ERROR) {
//				printf("send failed: %d\n", WSAGetLastError());
//				closesocket(clientSocket);
//				WSACleanup();
//				return 1;
//			}
//			printf("Bytes sent: %d\n", rs);
//		}
//		else if (r == 0)
//			printf("Connection closing...\n");
//		else {
//			printf("recv failed: %d\n", WSAGetLastError());
//			closesocket(clientSocket);
//			WSACleanup();
//			return 1;
//		}
//
//	} while (r > 0);
//}
//
//int Server::close() {
//	// shutdown the send half of the connection since no more data will be sent
//	int r = shutdown(clientSocket, SD_SEND);
//	if (r == SOCKET_ERROR) {
//		printf("shutdown failed: %d\n", WSAGetLastError());
//		closesocket(clientSocket);
//		WSACleanup();
//		return 1;
//	}
//
//	// cleanup
//	closesocket(clientSocket);
//	WSACleanup();
//}
//
