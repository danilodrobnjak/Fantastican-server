#include "ClientInChat.h"


State* ClientInChat::nextState() {

	if (endConversation) {
		std::cout << "NotConnected" << std::endl;
		return new NotConnected(m_clientSocket);
	}
	else {
		std::cout << "ClientInChat" << std::endl;
		return this;
	}
}


void  ClientInChat::messageCome(SOCKET& client, std::string& message) {

//	std::cout << " ClientInChat " << message << std::endl;
	if (!message.compare("cao\n"))
		endConversation = true;
	
}