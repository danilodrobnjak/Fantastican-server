#include <iostream>
#include "Server.h"

#define DEFAULT_PORT "27015"
#define SERVER_NAME "localhost"


int main(int argv, char** argc) {


	Server server = Server(SERVER_NAME, DEFAULT_PORT);

	std::thread Serverska_nit(&Server::run, server);

	Serverska_nit.join();

	std::cout << "Main nit" << std::endl;

}