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
//  Server
//	Version 0.1 By Rory Hemmings
//
//	1. Initialize Winsock.
//	1.5 Get server info
//  2. Create a socket.
//  3. Bind the socket.
//  4. Listen on the socket for a client.
//  5. Accept a connection from a client.
//  6. Receiveand send data.
//  7. Disconnect.
//

#include <iostream>
#include <string>

#include <SocketWrapper.h>

#include <windows.h>

#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define LOGO "DDDDDDDDDDDDD          iiii                                     \nD::::::::::::DDD      i::::i                                    \nD:::::::::::::::DD     iiii                                     \nDDD:::::DDDDD:::::D                                             \n  D:::::D    D:::::D iiiiiii nnnn  nnnnnnnn       ooooooooooo   \n  D:::::D     D:::::Di:::::i n:::nn::::::::nn   oo:::::::::::oo \n  D:::::D     D:::::D i::::i n::::::::::::::nn o:::::::::::::::o\n  D:::::D     D:::::D i::::i nn:::::::::::::::no:::::ooooo:::::o\n  D:::::D     D:::::D i::::i   n:::::nnnn:::::no::::o     o::::o\n  D:::::D     D:::::D i::::i   n::::n    n::::no::::o     o::::o\n  D:::::D     D:::::D i::::i   n::::n    n::::no::::o     o::::o\n  D:::::D    D:::::D  i::::i   n::::n    n::::no::::o     o::::o\nDDD:::::DDDDD:::::D  i::::::i  n::::n    n::::no:::::ooooo:::::o\nD:::::::::::::::DD   i::::::i  n::::n    n::::no:::::::::::::::o\nD::::::::::::DDD     i::::::i  n::::n    n::::n oo:::::::::::oo \nDDDDDDDDDDDDD        iiiiiiii  nnnnnn    nnnnnn   ooooooooooo   \n"

using namespace std;

void printLogo()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12);

	printf(LOGO);

	SetConsoleTextAttribute(hConsole, 15);
}

int main(int argc, char** argv)
{
	printLogo();

	ServerSocket server(DEFAULT_PORT);

	Socket* client = server.accept();

	string r = client->recieveLine();
	cout << r << endl;

	client->sendLine(r);

	return 0;
}
