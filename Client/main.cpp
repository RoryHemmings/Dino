//                                                                  
//                                                                  
//  DDDDDDDDDDDDD          iiii                                     
//  D::::::::::::DDD      i::::i                                    
//  D:::::::::::::::DD     iiii                                     
//  DDD:::::DDDDD:::::D                                             
//    D:::::D    D:::::D iiiiiii nnnn  nnnnnnnn       ooooooooooo   
//    D:::::D     D:::::Di:::::i n:::nn::::::::nn   oo:::::::::::oo 
//    D:::::D     D:::::D i::::i n::::::::::::::nn o:::::::::::::::o
//    D:::::D     D:::::D i::::i nn:::::::::::::::no:::::ooooo:::::o
//    D:::::D     D:::::D i::::i   n:::::nnnn:::::no::::o     o::::o
//    D:::::D     D:::::D i::::i   n::::n    n::::no::::o     o::::o
//    D:::::D     D:::::D i::::i   n::::n    n::::no::::o     o::::o
//    D:::::D    D:::::D  i::::i   n::::n    n::::no::::o     o::::o
//  DDD:::::DDDDD:::::D  i::::::i  n::::n    n::::no:::::ooooo:::::o
//  D:::::::::::::::DD   i::::::i  n::::n    n::::no:::::::::::::::o
//  D::::::::::::DDD     i::::::i  n::::n    n::::n oo:::::::::::oo 
//  DDDDDDDDDDDDD        iiiiiiii  nnnnnn    nnnnnn   ooooooooooo   
//                                                                  
//   
//	Client
//	Version 0.1 By Rory Hemmings
//
//	1. Initialize Winsock.
//	1.5 Get server info
//  2. Create a socket.
//  3. Connect to the server.
//  4. Send and receive data.
//  5. Disconnect.
//

#include <iostream>
#include <string>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

using namespace std;

int main(int argc, char** argv)
{
	if (argc != 2) {
		printf("Usage: %s <ADRESS>\n", argv[0]);
		return 1;
	}

	SOCKET connectSocket = INVALID_SOCKET;
	WSADATA wsaData;
	int r;

	// Initialize Winsock
	r = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (r != 0) {
		printf("WSAStartup failed: %d\n", r);
		return 1;
	}

	struct addrinfo *result = NULL, *ptr = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the local address and port to be used by the server, Take data from 'hints' struct, create addrinfo, and put it into 'result' struct
	r = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	if (r) {
		printf("getaddrinfo failed: %d\n", r);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to adress until one succeeds or result runs out of adresses
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create socket to connecting to server
		connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (connectSocket == INVALID_SOCKET) {
			printf("Error at socket(): %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			return 1;
		}

		// Connect socket to server
		r = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (r == SOCKET_ERROR) {
			closesocket(connectSocket);
			connectSocket = INVALID_SOCKET;
		}
	}

	// deallocate space for adress info
	freeaddrinfo(result);
	
	if (connectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	int recvbuflen = DEFAULT_BUFLEN;
	const char* sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	
	// Send buffer
	r = send(connectSocket, sendbuf, (int) strlen(sendbuf), 0);
	if (r == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(connectSocket);
		WSACleanup();
		return 1;
	}

	printf("Bytes Sent: %ld\n", r);

	// Shutdown the sending abilities of the socket (can still recieve)
	r = shutdown(connectSocket, SD_SEND);
	if (r == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(connectSocket);
		WSACleanup();
		return 1;
	}

	// Recieve data until server closes
	do {
		r = recv(connectSocket, recvbuf, recvbuflen, 0);
		if (r > 0)
			printf("Bytes received: %d\n", r);
		else if (r == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());
	} while (r > 0);

	// Close socket
	closesocket(connectSocket);
	WSACleanup();

	return 0;
}
