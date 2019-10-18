#include "Socket.h"
#include "globals.h"

using namespace std;

Socket::Socket(bool nonBlocking) : s(INVALID_SOCKET), nonBlocking(nonBlocking)
{
	start();

	// Use ipv4, and tcp
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) {
		stop();
		throw "Socket could not be created";
	}
}

string Socket::recieveLine() const
{
	string ret;
	while (true) {
		char c;

		// Recieve 1 character at a time
		int r = recv(s, &c, 1, 0);
		if (r == 0) {
			// disconnected
			return "!disconnected";
		} else if (r == -1) {
			return "!error";
		}

		if (c == '\n') return ret;
		ret += c;
	}
}

string Socket::recieveBytes() const
{
	string ret;
	char buf[DEFAULT_BUFLEN];

	while (true) {
		unsigned long arg = 0;
		if (ioctlsocket(s, FIONREAD, &arg) != 0 || arg == 0) break;
		arg = (arg > DEFAULT_BUFLEN) ? DEFAULT_BUFLEN : arg;

		int rv = recv(s, buf, arg, 0);
		if (rv <= 0) break;

		string t;
		t.assign(buf, rv);
		ret += t;
	}

	return ret;
}

void Socket::sendLine(string message) const
{
	message += '\n';
	sendBytes(message);
}

void Socket::sendBytes(const string& message) const
{
	::send(s, message.c_str(), message.length(), 0);
}

void Socket::start()
{
	WSADATA i;
	if (WSAStartup(MAKEWORD(2, 2), &i)) {
		stop();
		throw "Couse Not Start WSA";
	}
}

void Socket::stop()
{
	WSACleanup();
}

string Socket::getAddr() const
{
	SOCKADDR_IN info = { 0 };
	int addrsize = sizeof(info);
	getpeername(s, (struct sockaddr*) & info, &addrsize);

	char ret[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(info.sin_addr), ret, INET_ADDRSTRLEN);
	return string(ret);
}

ServerSocket::ServerSocket(int port, bool nonBlocking, int backlog) : Socket(nonBlocking)
{
	struct addrinfo* result = NULL, * ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	
	// Get availible addresses
	if (getaddrinfo(NULL, to_string(port).c_str(), &hints, &result)) { freeaddrinfo(result); stop(); throw "addrinfo failed"; }
	
	// Create Socket
	s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (s == INVALID_SOCKET) { freeaddrinfo(result); stop(); throw "Invalid Socket"; }

	
	// Set to Non-Blocking
	if (nonBlocking) {
		u_long arg = 1;
		ioctlsocket(s, FIONBIO, &arg);
	}

	// Bind
	if (bind(s, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) { freeaddrinfo(result); close(); stop(); throw "Failed to bind";  }
	freeaddrinfo(result);

	listen(s, backlog);

	struct sockaddr_in sin;
	socklen_t len = sizeof(sin);
	if (getsockname(s, (struct sockaddr*) &sin, &len) == -1)
		perror("getsockname");
	else
		printf("Listening on port %d\n", ntohs(sin.sin_port));
}

// NON-BLOCKING (call repeatedly) returns 0 if no request
Socket* ServerSocket::accept()
{
	// Accept new connection
	SOCKET n = ::accept(s, 0, 0);
	if (n == INVALID_SOCKET) {
		int rc = WSAGetLastError();
		if (rc == WSAEWOULDBLOCK) return 0; // Non-Blocking call, no request
		else throw "Invalid Socket";
	}

	Socket* ret = new Socket(n);
	return ret;
}

ClientSocket::ClientSocket(const string& host, int port, bool nonBlocking) : Socket(nonBlocking)
{
	s = INVALID_SOCKET;

	struct addrinfo* result = NULL, * ptr = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(host.c_str(), to_string(port).c_str(), &hints, &result)) { close(); throw "getaddrinfo failed"; }

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		s = socket(AF_INET, SOCK_STREAM, 0);
		if (s == INVALID_SOCKET) {
			stop();
			throw "Socket could not be created";
		}
		if (::connect(s, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR) { close(); s = INVALID_SOCKET; }
	}

	if (nonBlocking) {
		u_long arg = 1;
		ioctlsocket(s, FIONBIO, &arg);
	}

	freeaddrinfo(result);

	if (s == INVALID_SOCKET) { close(); throw "Unable to connect to server"; }
}

