#pragma once
#include "State.h"
#include "NotInChat.h"
#include "NotConnected.h";


#define DEFAULT_BUFLEN 512

#pragma comment (lib, "Ws2_32.lib")

class HaveRequestForConnection : public NotInChat {

public:

	HaveRequestForConnection(SOCKET& client) : m_clientSocket(client) {};;

	State* nextState() override;
	void messageCome(SOCKET& client, std::string& message) override;



private: 
	std::string m_response = "";
	SOCKET m_clientSocket;
};