#pragma once
#ifndef _CLIENTINCHAT_H_
#define _CLIENTINCHAT_H_
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include "NotConnected.h"
#include "LeftApp.h"

#define DEFAULT_BUFLEN 512

#pragma comment (lib, "Ws2_32.lib")

class ClientInChat : public State {

public:

	ClientInChat(SOCKET& client, std::shared_ptr<SharedElement> socketToProcess);

	State* nextState() override;
	void messageCome(SOCKET &client, std::string& message) override;

	bool isInChat();


private:
	bool endConversation = false;
	SOCKET m_clientSocket;

};
#endif