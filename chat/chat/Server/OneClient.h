#pragma once
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include<string>
#include<iostream>
#include<thread>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define DEFAULT_BUFLEN 512

#pragma comment (lib, "Ws2_32.lib")


class OneClient {

public:
	OneClient(SOCKET& ClientSocket);
	~OneClient();
	void run();

	bool waitingOtherToConnenct;

	__event void NewComeToChat(OneClient &client);
	__event void tryToConnect(OneClient& client,const std::string &name);

	std::string getName();
	SOCKET getSocket();

	void sendOnlineClients(std::string message);
	void notAlone();
	void read(const SOCKET ConnectSocket);
	void write(const SOCKET ConnectSocket);

	friend bool operator!=(OneClient& one, OneClient& two);

private:

	std::string name;
	SOCKET ClientSocket;
	char sendbuf[DEFAULT_BUFLEN];
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	int sendbuflen = DEFAULT_BUFLEN;
	bool end = false;
    
	
	
};

