#include "ClientRegistration.h"
#include  "NotConnected.h"


ClientRegistration::ClientRegistration(SOCKET& client,std::string *name)
	:m_name(name),m_clientSocket(client){

}


State* ClientRegistration::nextState() {
	std::cout << "NotConnected" << std::endl;
	return new NotConnected(m_clientSocket);
}



void ClientRegistration::messageCome(SOCKET& client, std::string &message) {

	//std::cout << "ClientRegistration "<< message << std::endl;
	*m_name = message;

}