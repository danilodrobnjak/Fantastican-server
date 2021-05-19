#pragma once
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


#define DEFAULT_BUFLEN 512

#pragma comment (lib, "Ws2_32.lib")

class ClientRegistration : public State{

public:
	
	ClientRegistration(SOCKET& client,std::string *name);
	
     State* nextState() override;
	 void messageCome(SOCKET& client, std::string& message) override;

	 bool isInChat()  {
		 return true;
	 }

private:
	std::string *m_name;
	SOCKET m_clientSocket;

};