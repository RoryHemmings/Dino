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

#include <SocketWrapper.h>

#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main(int argc, char** argv)
{
	if (argc != 2) {
		printf("Usage: %s <ADRESS>\n", argv[0]);
		return 1;
	}

	ClientSocket client(argv[1], DEFAULT_PORT);
	client.sendLine("First");

	while (1) {
		string l = client.recieveLine();
		if (l.empty()) break;
		cout << l;
		cout.flush();
	}

	return 0;
}
