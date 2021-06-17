#pragma once
#ifndef _NOTINCHAT_
#define _NOTINCHAT_
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include "State.h"
#include "LeftApp.h"
#define DEFAULT_BUFLEN 512

#pragma comment (lib, "Ws2_32.lib")

class NotInChat : public State {

public:

	NotInChat(std::shared_ptr<SharedElement> socketToProcess)
	:State(socketToProcess){

	}
	virtual ~NotInChat() {};
	virtual State* nextState() = 0;
	virtual void messageCome(SOCKET& client, std::string& message) = 0;

	bool isInChat() {
		return false;
	}

};
#endif // !_NOTINCHAT_

