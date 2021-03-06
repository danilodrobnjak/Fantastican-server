#pragma once
#ifndef _WRITE_H
#define _WRITE_H


#define WIN32_LEAN_AND_MEAN
#define DEFAULT_BUFLEN 512
#include <iostream>
#include <thread>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include<mutex>
#define _CRT_SECURE_NO_WARNINGS

#pragma comment (lib, "Ws2_32.lib")
class Write {

public:
	
	Write(SOCKET& clientSocket);

	void run();
	void messageToWrite(const std::string& message);


//private:
	char m_sendbuf[DEFAULT_BUFLEN];
	int m_sendbuflen = DEFAULT_BUFLEN;
	SOCKET m_clientSocket;
	int m_iResult;
	bool m_end;
	bool m_newMessage=false;
	std::mutex mtx;
};
#endif // !_WRITE_H