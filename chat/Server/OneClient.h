#pragma once
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include<string>
#include<iostream>
#include<thread>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include<mutex>
#include "Read.h"
#include "Write.h"
#include "State.h"
#include "NotInChat.h"
#include "ClientRegistration.h"

#define DEFAULT_BUFLEN 512

#pragma comment (lib, "Ws2_32.lib")


class OneClient {

public:
	OneClient(SOCKET& ClientSocket);
	~OneClient();
	void run();

	__event void NewComeToChat(OneClient &client);
	__event void TryToConnect(OneClient &client,std::string &name);
	__event void ResponseToConnect(OneClient& client,int value);
	__event void ChatMessage(OneClient& client, std::string& name);
	__event void ClientLeftTheChat(OneClient& client);
	__event void DeleteClient(OneClient& client);
	
	std::string getName();
	SOCKET& getSocket();
	bool getWaitingOtherToConnenct();
	void setWaitingOtherToConnenct(bool waitingOtherToConnenct) {
		m_waitingOtherToConnenct = waitingOtherToConnenct;
	}
	State* getState() {
		return m_state;
	}

	void setState(State * state){
		m_state = state;
	};
	void setCase(int nCase);
	void sendMessageToClient(std::string &message);
	void messageCome( std::string &message);
	void sendOnlineClients(std::string message);
	void notAlone();
	friend bool operator!=(const OneClient &one,const OneClient &two);

	//State* m_state;

private:

	std::string m_name ;
	SOCKET m_ClientSocket;
	char m_sendbuf[DEFAULT_BUFLEN];
	char m_recvbuf[DEFAULT_BUFLEN];
	int m_recvbuflen = DEFAULT_BUFLEN;
	int m_sendbuflen = DEFAULT_BUFLEN;
	bool m_end = false;
	bool m_waitingOtherToConnenct;
	int m_case = 0;
    std::shared_ptr<Read> m_read;
	std::shared_ptr<Write> m_write;

	//state of object
	State* m_state;

	void hookMessageCome(std::shared_ptr<Read> read);

};

