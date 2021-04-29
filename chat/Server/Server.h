#pragma once
#define WIN32_LEAN_AND_MEAN
#include "OneClient.h"
#include <string>
#include <iostream>
#include <thread>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

class Server  {

public:
	Server(std::string name, std::string port);
	~Server();
	void run();
	void initWSA();

	void addNewClient(OneClient &client);
	void hookAddNewClient(std::shared_ptr<OneClient> client);
	void tryToConnect(OneClient& client,std::string &name);
	void hookTryToConnect(std::shared_ptr<OneClient> &client);

private:
	std::string name;
	std::string port;
	SOCKET ListenSocket;
	std::vector<std::shared_ptr<OneClient>> clients;
	std::vector<std::pair<std::shared_ptr<OneClient>, std::shared_ptr<OneClient>>> connectedClients;
	
};

