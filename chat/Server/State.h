#pragma once
#ifndef _STATE_H
#define _STATE_H

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <winsock2.h>
#include<iostream>
#include <string>
#include <thread>
#include <ws2tcpip.h>
#include "SharedElement.h"
#include "Log.h"



#define DEFAULT_BUFLEN 512

#pragma comment (lib, "Ws2_32.lib")

class State {

public:

	State(std::shared_ptr<SharedElement>& socketToProcess)
	:socketToProcess(socketToProcess){
	};
	

	virtual ~State() {}

	virtual State* nextState() = 0;
	virtual void messageCome(SOCKET& client, std::string& message) = 0;

	virtual bool isInChat() = 0;

	bool m_clientLeftApp = false;

	std::shared_ptr<SharedElement> socketToProcess;
};

#endif // !_STATE_H

