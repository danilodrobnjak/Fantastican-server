
#include "WaitForResponseToConnect.h"

State* WaitForResponseToConnect::nextState() {
	std::cout << " WaitForResponseToConnect " << std::endl;
	return this;
}


void  WaitForResponseToConnect::messageCome(SOCKET& client, std::string& message) {

	//std::cout << " WaitForResponseToConnect " << message << std::endl;

}