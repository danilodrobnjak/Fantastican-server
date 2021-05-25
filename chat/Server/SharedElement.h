#pragma once
#ifndef _SHAREDELEMENT_H_
#define _SHAREDELEMENT_H_
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mutex>
#pragma comment (lib, "Ws2_32.lib")

class SharedElement {

public: 
	SharedElement(SOCKET socketToProcess)
		: m_socketToProcess(socketToProcess){
	}
	SharedElement(const SharedElement& copy) {
	}
	~SharedElement() {

	}
	//NotConn
	std::atomic<SOCKET> m_socketToProcess;
	std::mutex  m_socketToProcess_mutex;
	std::condition_variable m_socketToProcess_cv;
	bool m_socketToProcess_ready = false;
	//WaitFor
	std::string m_nameToProcess;
	SOCKET m_nameSocketToProcess;
	std::mutex  m_nameToProcess_mutex;
	std::condition_variable m_nameToProcess_cv;
	bool m_nameToProcess_ready = false;
	//HaveReq
	int m_responseToProcess;
	SOCKET m_responseSocketToProcess;
	std::mutex  m_responseToProcess_mutex;
	std::condition_variable m_responseToProcess_cv;
	bool m_responseToProcess_ready = false;
	//ClientChat
	std::string m_chatMessageToProcess;
	SOCKET m_chatMessageSocketToProcess;
	std::mutex  m_chatMessageToProcess_mutex;
	std::condition_variable m_chatMessageToProcess_cv;
	bool m_chatMessageToProcess_ready = false;

	//ClientChat cao
	SOCKET m_leftChatSocketToProcess;
	std::mutex  m_leftChatToProcess_mutex;
	std::condition_variable m_leftChatToProcess_cv;
	bool m_leftChatToProcess_ready = false;

	//ClientChat q
	SOCKET m_clientLeftAppSocketToProcess;
	std::mutex  m_clientLeftAppToProcess_mutex;
	std::condition_variable m_clientLeftAppToProcess_cv;
	bool m_clientLeftAppToProcess_ready = false;


};


#endif // !_SHAREDELEMENT_H_
