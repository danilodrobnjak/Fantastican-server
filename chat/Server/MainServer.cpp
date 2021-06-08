#include <iostream>
#include "Server.h"

#define DEFAULT_PORT "27015"
#define SERVER_NAME "localhost"

/*
class parser
{
public:
	parser(int argv, char** argc);
	bool is_input_valid()
};

void usage()
{
	const char* message = "Usage:\n - -p port"
}
*/

int main(int argv, char** argc) {

	bool initWSA = false; 

	//parser p(argv);
	//if (!p.is_input_valid())
	//{
	//	usage();
	//	return EXIT_FAILURE;
	//}
	Server server(SERVER_NAME, DEFAULT_PORT, initWSA);

	if (initWSA) {
		std::cout << "Server start.... " << std::endl;
	}
	else {
		std::cout <<"WSAStartup failed " << std::endl;
	}



}