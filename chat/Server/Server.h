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
#include <unordered_map>
#include <tuple>
#include <mutex>

#pragma comment (lib, "Ws2_32.lib")

class Server  {

public:
	Server(std::string name, std::string port);
	~Server();
	void run();


private:
	
	std::string m_name;
	std::string m_port;
	SOCKET m_ListenSocket;
	std::vector<std::shared_ptr<OneClient>> m_clients;
	//std::mutex m_clients_mutex;
	std::vector <std::tuple<std::shared_ptr<OneClient>, std::shared_ptr<OneClient>,int>> m_connectedClients;
	//std::mutex m_connectedClients_mutex;

	void initWSA();
	void addNewClient(OneClient& client);
	void hookAddNewClient(std::shared_ptr<OneClient> client);
	void tryToConnect(OneClient &client, std::string& name);
	void hookTryToConnect(std::shared_ptr<OneClient>& client);
	void responseToConnect(OneClient& client, int value);
	void hookResponseToConnect(std::shared_ptr<OneClient> client);
	void chatMessage(OneClient& client, std::string& message);
	void hookChatMessage(std::shared_ptr<OneClient>& client);
};

