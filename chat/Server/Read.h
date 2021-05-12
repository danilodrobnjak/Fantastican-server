#pragma once
#define WIN32_LEAN_AND_MEAN
#define DEFAULT_BUFLEN 512
#include <iostream>
#include <thread>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>


#pragma comment (lib, "Ws2_32.lib")
class Read {

public: 
	Read(SOCKET &clientSocket);

	void run();

	__event void MessageCome( std::string &message);

private:
	char m_recvbuf[DEFAULT_BUFLEN];
	int m_recvbuflen = DEFAULT_BUFLEN;
	SOCKET m_clientSocket;
	int m_iResult;
	bool m_end;
};
