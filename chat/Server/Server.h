#pragma once
#ifndef _SERVER_H
#define _SERVER_H
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
#include <shared_mutex>
#include "HaveRequestForConnection.h"
#include "NotConnected.h";
#include "ClientInChat.h"
#include "SharedElement.h"
#define DEFAULT_BUFLEN 512

#pragma comment (lib, "Ws2_32.lib")

class Server  {

public:
	Server(std::string name, std::string port,bool &initWsa);	
	~Server();


private:
	
	std::string m_name;
	std::string m_port;
	SOCKET m_ListenSocket;

	std::shared_mutex m_clients_mutex;
	std::vector<std::shared_ptr<OneClient>> m_clients;
	std::shared_mutex m_connectedClients_mutex;
	std::vector <std::pair<std::shared_ptr<OneClient>, std::shared_ptr<OneClient>>> m_connectedClients;
	std::shared_ptr<SharedElement> socketToProcess = std::make_shared<SharedElement>(SharedElement());

	std::thread  m_runThread;
	std::thread  m_addNewClientThread;
	std::thread  m_tryToConnectThread;
	std::thread  m_responseToConnectThread;
	std::thread  m_chatMessageThread;
	std::thread  m_clientLeftTheChatThread;
	std::thread  m_deleteClient;
	

	bool initWSA();
	void run();
	void addNewClient();
	void tryToConnect();
	void responseToConnect();
	void chatMessage();
	void clientLeftTheChat();
	void deleteClient();
	
};
#endif

