#include "HaveRequestForConnection.h"
#include "ClientInChat.h"
State* HaveRequestForConnection::nextState() {

	if (!m_response.compare("da\n")) {
		std::cout << "ClientInChat" << std::endl;
		return new ClientInChat(m_clientSocket);
	}
	else {
		std::cout << "NotConnected" << std::endl;
		return new NotConnected(m_clientSocket);
	}
}


void  HaveRequestForConnection::messageCome(SOCKET& client, std::string& message) {

	//std::cout << " HaveRequestForConnection " << message << std::endl;

	m_response = message;
}