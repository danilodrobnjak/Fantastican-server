#pragma once
#ifndef _WAITFORRESTOCONN_
#define _WAITFORRESTOCONN_
#include "NotInChat.h"

#define DEFAULT_BUFLEN 512

#pragma comment (lib, "Ws2_32.lib")

class WaitForResponseToConnect : public NotInChat {

public:

	WaitForResponseToConnect (SOCKET& client, std::shared_ptr<SharedElement> socketToProcess);

	State* nextState() override;
	void messageCome(SOCKET& client, std::string& message) override;

	SOCKET m_clientSocket;

};
#endif