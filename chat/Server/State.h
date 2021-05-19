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


#define DEFAULT_BUFLEN 512

#pragma comment (lib, "Ws2_32.lib")

class State {

public:
	//__event void NewComeToChat(SOCKET& client);
	 State() = default;

	 virtual ~State() {
	 }

	virtual State* nextState() = 0;
	virtual void messageCome(SOCKET& client, std::string& message)= 0;

	virtual bool isInChat() = 0;

};