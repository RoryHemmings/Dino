////                                                                 
////   
////  DDDDDDDDDDDDD          iiii                                     
////  D::::::::::::DDD      i::::i                                    
////  D:::::::::::::::DD     iiii                                     
////  DDD:::::DDDDD:::::D                                             
////    D:::::D    D:::::D iiiiiii nnnn  nnnnnnnn       ooooooooooo   
////    D:::::D     D:::::Di:::::i n:::nn::::::::nn   oo:::::::::::oo 
////    D:::::D     D:::::D i::::i n::::::::::::::nn o:::::::::::::::o
////    D:::::D     D:::::D i::::i nn:::::::::::::::no:::::ooooo:::::o
////    D:::::D     D:::::D i::::i   n:::::nnnn:::::no::::o     o::::o
////    D:::::D     D:::::D i::::i   n::::n    n::::no::::o     o::::o
////    D:::::D     D:::::D i::::i   n::::n    n::::no::::o     o::::o
////    D:::::D    D:::::D  i::::i   n::::n    n::::no::::o     o::::o
////  DDD:::::DDDDD:::::D  i::::::i  n::::n    n::::no:::::ooooo:::::o
////  D:::::::::::::::DD   i::::::i  n::::n    n::::no:::::::::::::::o
////  D::::::::::::DDD     i::::::i  n::::n    n::::n oo:::::::::::oo 
////  DDDDDDDDDDDDD        iiiiiiii  nnnnnn    nnnnnn   ooooooooooo   
////                                                                  
////      
////  Server
////	Version 0.1 By Rory Hemmings
////
////	1. Initialize Winsock.
////	1.5 Get server info
////  2. Create a socket.
////  3. Bind the socket.
////  4. Listen on the socket for a client.
////  5. Accept a connection from a client.
////  6. Receiveand send data.
////  7. Disconnect.
////
//
//#include <iostream>
//#include <string>
//
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <iphlpapi.h>
//
//#include <windows.h>
//#include <stdio.h>
//
//#pragma comment(lib, "Ws2_32.lib")
//
//#define LOGO "DDDDDDDDDDDDD          iiii                                     \nD::::::::::::DDD      i::::i                                    \nD:::::::::::::::DD     iiii                                     \nDDD:::::DDDDD:::::D                                             \n  D:::::D    D:::::D iiiiiii nnnn  nnnnnnnn       ooooooooooo   \n  D:::::D     D:::::Di:::::i n:::nn::::::::nn   oo:::::::::::oo \n  D:::::D     D:::::D i::::i n::::::::::::::nn o:::::::::::::::o\n  D:::::D     D:::::D i::::i nn:::::::::::::::no:::::ooooo:::::o\n  D:::::D     D:::::D i::::i   n:::::nnnn:::::no::::o     o::::o\n  D:::::D     D:::::D i::::i   n::::n    n::::no::::o     o::::o\n  D:::::D     D:::::D i::::i   n::::n    n::::no::::o     o::::o\n  D:::::D    D:::::D  i::::i   n::::n    n::::no::::o     o::::o\nDDD:::::DDDDD:::::D  i::::::i  n::::n    n::::no:::::ooooo:::::o\nD:::::::::::::::DD   i::::::i  n::::n    n::::no:::::::::::::::o\nD::::::::::::DDD     i::::::i  n::::n    n::::n oo:::::::::::oo \nDDDDDDDDDDDDD        iiiiiiii  nnnnnn    nnnnnn   ooooooooooo   \n"
//
//#define DEFAULT_BUFLEN 512
//#define DEFAULT_PORT "27015"
//
//using namespace std;
//
//void printLogo()
//{
//	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//	SetConsoleTextAttribute(hConsole, 12);
//
//	printf(LOGO);
//
//	SetConsoleTextAttribute(hConsole, 15);
//}
//
//int main(int argc, char** argv)
//{
//	printLogo();
//
//	WSADATA wsaData;
//
//	// Initialize Winsock
//	int r;
//	r = WSAStartup(MAKEWORD(2, 2), &wsaData);
//	if (r) {
//		printf("WSAStartup failed: %d\n", r);
//		return 1;
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
//	r = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
//	if (r) {
//		printf("getaddrinfo failed: %d\n", r);
//		WSACleanup();
//		return 1;
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
//		return 1;
//	}
//
//	// Bind Socket
//	r = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
//	if (r == SOCKET_ERROR) {
//		printf("bind failed with error: %d\n", WSAGetLastError());
//		freeaddrinfo(result);
//		closesocket(listenSocket);
//		WSACleanup();
//		return 1;
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
//		return 1;
//	}
//
//	// Temporary variable to keep track of Client connection
//	SOCKET clientSocket = INVALID_SOCKET;
//
//	clientSocket = accept(listenSocket, NULL, NULL);
//	if (clientSocket == INVALID_SOCKET) {
//		printf("accept failed: %d\n", WSAGetLastError());
//		closesocket(listenSocket);
//		WSACleanup();
//		return 1;
//	}
//
//	char recvbuf[DEFAULT_BUFLEN];
//	int rs;
//	int recvbuflen = DEFAULT_BUFLEN;
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
//
//	// shutdown the send half of the connection since no more data will be sent
//	r = shutdown(clientSocket, SD_SEND);
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
//
//	return 0;
//}