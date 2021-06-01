#pragma once
#ifndef _SHAREDELEMENT_H_
#define _SHAREDELEMENT_H_
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mutex>
#pragma comment (lib, "Ws2_32.lib")
#include "SafeQueue.h"

class SharedElement {

public: 
	SharedElement()	{
	}
	SharedElement(const SharedElement& copy) {
	}
	~SharedElement() {
	}
	//NotConn
	SafeQueue<SOCKET> m_socketToProcess;
	
	//NotConn
	SafeQueue<std::pair<std::string,SOCKET>> m_nameToProcess;
	

	//HaveReq
	SafeQueue<std::pair<int, SOCKET>> m_responseToProcess;
	 
	//ClientChat
	SafeQueue<std::pair<std::string, SOCKET>> m_chatMessageToProcess;


	//ClientChat cao
	SafeQueue<SOCKET>  m_leftChatToProcess;
	

	//ClientChat q
	SafeQueue<SOCKET>  m_clientLeftAppToProcess;


};


#endif // !_SHAREDELEMENT_H_
