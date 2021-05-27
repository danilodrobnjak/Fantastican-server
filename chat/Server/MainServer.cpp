#include <iostream>
#include "Server.h"

#define DEFAULT_PORT "27015"
#define SERVER_NAME "localhost"


int main(int argv, char** argc) {

	bool initWSA = false; 

	Server server(SERVER_NAME, DEFAULT_PORT ,initWSA);

	if (initWSA) {
		std::thread Serverska_nit(&Server::run, &server);
		std::thread Serverska_nit2(&Server::addNewClient, &server);
		std::thread Serverska_nit3(&Server::tryToConnect, &server);
		std::thread Serverska_nit4(&Server::responseToConnect, &server);
		std::thread Serverska_nit5(&Server::chatMessage, &server);
		std::thread Serverska_nit6(&Server::clientLeftTheChat, &server);
		std::thread Serverska_nit7(&Server::deleteClient, &server);
		Serverska_nit.join();
		Serverska_nit2.join();
		Serverska_nit3.join();
		Serverska_nit4.join();
		Serverska_nit5.join();
		Serverska_nit6.join();
		Serverska_nit7.join();
	}
	else {
		std::cout <<"WSAStartup failed " << std::endl;
	}



}