#pragma once
#ifndef _LEFT_APP_H_
#define _LEFT_APP_H_
#include "State.h"

class LeftApp :public State {


public:
	LeftApp(SOCKET &client,std::shared_ptr<SharedElement>& socketToProcess) 
		:State(socketToProcess){
		std::unique_lock<std::mutex> ul(socketToProcess.get()->m_clientLeftAppToProcess_mutex);

		(*socketToProcess.get()).m_clientLeftAppSocketToProcess = client;

		socketToProcess.get()->m_clientLeftAppToProcess_ready = true;

		ul.unlock();

		socketToProcess.get()->m_clientLeftAppToProcess_cv.notify_one();
	
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
