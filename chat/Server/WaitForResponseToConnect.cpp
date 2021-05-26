
#include "WaitForResponseToConnect.h"

WaitForResponseToConnect::WaitForResponseToConnect(SOCKET& client, std::shared_ptr<SharedElement>& socketToProcess)
	:NotInChat(socketToProcess), m_clientSocket(client) {
}


State* WaitForResponseToConnect::nextState() {
	std::cout << " WaitForResponseToConnect " << std::endl;
	if (m_clientLeftApp) {
		return new LeftApp(m_clientSocket, this->socketToProcess);
	}
	else {
		return this;
	}
}


void  WaitForResponseToConnect::messageCome(SOCKET& client, std::string& message) {

	if (!message.compare("q\n")) {
		m_clientLeftApp = true;
	}

}