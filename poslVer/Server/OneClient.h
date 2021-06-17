#pragma once
#ifndef _ONECLIENT_H
#define _ONECLIENT_H
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include<string>
#include<iostream>
#include<thread>
#include <cstdio>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include<mutex>
#include "Read.h"
#include "Write.h"
#include "State.h"
#include "NotInChat.h"
#include "ClientRegistration.h"
#include "SharedElement.h"
#define DEFAULT_BUFLEN 512

#pragma comment (lib, "Ws2_32.lib")


class OneClient {

public:
	OneClient(SOCKET& ClientSocket, std::shared_ptr<SharedElement> socketToProcess);
	OneClient() = default;
	~OneClient();

	std::string getName();
	SOCKET& getSocket();
	State* getState();
	void setState(State* state);


	void sendMessageToClient(std::string& message);
	void messageCome(std::string& message);
	friend bool operator!=(const OneClient& one, const OneClient& two);


private:

	std::string m_name;
	SOCKET m_ClientSocket;
	bool m_end = false;
	State* m_state;
	std::shared_ptr<SharedElement> m_socketToProcess;
	std::shared_ptr<Read> m_read;
	std::shared_ptr<Write> m_write;
	std::thread citanje;
	std::thread pisanje;


	void hookMessageCome(std::shared_ptr<Read> read);

};

#endif // !_ONECLIENT_H



