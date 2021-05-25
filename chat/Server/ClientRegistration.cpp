#include "ClientRegistration.h"



ClientRegistration::ClientRegistration(SOCKET& client,std::string *name, std::shared_ptr<SharedElement>& socketToProcess)
	:State(socketToProcess),m_name(name),m_clientSocket(client){

}


State* ClientRegistration::nextState() {
	std::cout << "NotConnected" << std::endl;
	if (m_clientLeftApp) {
		return new LeftApp(m_clientSocket, this->socketToProcess);
	}
	else {
		return new NotConnected(m_clientSocket, this->socketToProcess);
	}
}



void ClientRegistration::messageCome(SOCKET& client, std::string &message) {

	//std::cout << "ClientRegistration "<< message << std::endl;
	*m_name = message;
	if (!message.compare("q\n")) {
		m_clientLeftApp = true;
	}

}


bool ClientRegistration::isInChat() {
	return true;
}