#pragma once
#ifndef _LEFT_APP_H_
#define _LEFT_APP_H_
#include "State.h"

class LeftApp :public State {


public:
	LeftApp(SOCKET &client,std::shared_ptr<SharedElement> socketToProcess) 
		:State(socketToProcess){
		

		socketToProcess->m_clientLeftAppToProcess.enqueue(client);

	
	}
	State* nextState() {
		return this; 
	}
	void messageCome(SOCKET& client, std::string& message) {
	 }

	bool isInChat() {
		return true;
	 }
};
#endif // !_LEFT_APP_H_
