#include <iostream>
#include "Server.h"

#define DEFAULT_PORT "27015"
#define SERVER_NAME "localhost"


int main(int argv, char** argc) {

	bool initWSA = false; 

	Server server(SERVER_NAME, DEFAULT_PORT ,initWSA);

	if (initWSA) {
		std::thread Serverska_nit(&Server::run, server);
		std::thread Serverska_nit2(&Server::addNewClient2, server);
		Serverska_nit.join();
		Serverska_nit2.join();
	}
	else {
		std::cout <<"WSAStartup failed " << std::endl;
	}

	std::cout << "Main nit" << std::endl;

}