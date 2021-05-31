#pragma once
#ifndef _HAVEREQFORCONN_
#define _HAVEREQFORCONN_

#include "NotInChat.h"
#include "NotConnected.h";
#include "ClientInChat.h"

#define DEFAULT_BUFLEN 512

#pragma comment (lib, "Ws2_32.lib")

class HaveRequestForConnection : public NotInChat {

public:

	HaveRequestForConnection(SOCKET& client, std::shared_ptr<SharedElement>& socketToProcess);
	State* nextState() override;
	void messageCome(SOCKET& client, std::string& message) override;



private: 
	std::string m_response = "";
	SOCKET m_clientSocket;
};
#endif