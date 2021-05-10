#pragma once
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include<string>
#include<iostream>
#include<thread>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Read.h"
#include "Write.h"

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
	__event void MessageToWrite(const std::string& message);

	void hookmessageToWrite(OneClient* write);


	void messageCome(const std::string &message);
	void hookMessageCome(std::shared_ptr<Read> read);



	std::string getName();
	SOCKET getSocket();
	bool getWaitingOtherToConnenct();

	void sendOnlineClients(std::string message);
	void notAlone();

	friend bool operator!=(const OneClient &one,const OneClient &two);

	


private:

	std::string m_name;
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

    
	
	
};

