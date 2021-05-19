#pragma once
#include "State.h"
#include "NotInChat.h"


#define DEFAULT_BUFLEN 512

#pragma comment (lib, "Ws2_32.lib")

class WaitForResponseToConnect : public NotInChat {

public:

	WaitForResponseToConnect (SOCKET& client) : m_clientSocket(client) {};;

	State* nextState() override;
	void messageCome(SOCKET& client, std::string& message) override;

	SOCKET m_clientSocket;

};