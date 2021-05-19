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
#include "NotInChat.h"
//#include "Locks.h"

#define DEFAULT_BUFLEN 512

#pragma comment (lib, "Ws2_32.lib")

class NotConnected : public NotInChat {

public:
	

	 NotConnected(SOCKET& client);
	 State* nextState() override;
	 void messageCome(SOCKET& client, std::string& message) override;

	 SOCKET m_clientSocket;

	
};