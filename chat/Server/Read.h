#pragma once
#define DEFAULT_BUFLEN 512


class Read {

public: 
	Read() = default;


private:
	char recvbuf[DEFAULT_BUFLEN];


};
