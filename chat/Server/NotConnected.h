#pragma once
#ifndef _NOTCONN_
#define  _NOTCONN_
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include "NotInChat.h"
#include "WaitForResponseToConnect.h"


#define DEFAULT_BUFLEN 512

#pragma comment (lib, "Ws2_32.lib")

class NotConnected : public NotInChat {

public:
	

	NotConnected(SOCKET& client, std::shared_ptr<SharedElement> socketToProcess);
	 State* nextState() override;
	 void messageCome(SOCKET& client, std::string& message) override;

	 SOCKET m_clientSocket;

	
};
#endif