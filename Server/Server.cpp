#include "Server.h"

Server::Server(int port) 
	: socket(port), clients(), current() 
{ }


