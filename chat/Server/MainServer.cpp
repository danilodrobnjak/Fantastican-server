#include "Server.h"
#include "Parser.h"
#include"E_WsaFaildServer.h"

#define DEFAULT_PORT "27015"
#define SERVER_NAME "localhost"


void usage()
{
	const char* message = "Usage:\n  MainServer [--port -p] portValue\n []";
}
void initWSA() {

	WSADATA wsaData;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		throw new E_WsaFaildServer();
	}

	return;
}

int main(int argv, char** argc) {

		
		
	try
	{
		initWSA();
	}
	catch (E_WsaFaildServer&){
		std::cout << "WSAStartup failed " << std::endl;
	}
	std::cout << "Server start.... " << std::endl;

	
	
	Parser p(argv, argc);
	if (!p.isInputValid()) {
		usage();
		exit(0);
	}
	
	

	
	Server server(SERVER_NAME, DEFAULT_PORT);

	


}