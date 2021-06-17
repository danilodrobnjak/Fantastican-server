#pragma once
#ifndef _CLIENTREG_
#define _CLIENTREG_
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include  "NotConnected.h"
#include "LeftApp.h"
#define DEFAULT_BUFLEN 512

#pragma comment (lib, "Ws2_32.lib")

class ClientRegistration : public State{

public:
	
	ClientRegistration(SOCKET& client,std::string *name, std::shared_ptr<SharedElement> socketToProcess);
	
     State* nextState() override;
	 void messageCome(SOCKET& client, std::string& message) override;

	 bool isInChat();

private:
	std::string *m_name;
	SOCKET m_clientSocket;

};
#endif