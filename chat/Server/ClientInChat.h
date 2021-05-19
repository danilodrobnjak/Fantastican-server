#pragma once
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include<string>
#include<iostream>
#include<thread>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include<mutex>
#include "Read.h"
#include "Write.h"
#include "State.h"
#include "NotConnected.h"


#define DEFAULT_BUFLEN 512

#pragma comment (lib, "Ws2_32.lib")

class ClientInChat : public State {

public:

	ClientInChat(SOCKET& client) :m_clientSocket(client) {};

	State* nextState() override;
	void messageCome(SOCKET &client, std::string& message) override;

	bool isInChat()  {
		return true;
	}


private:
	bool endConversation = false;
	SOCKET m_clientSocket;

};
