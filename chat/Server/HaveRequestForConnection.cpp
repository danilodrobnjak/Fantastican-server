#include "HaveRequestForConnection.h"
HaveRequestForConnection::HaveRequestForConnection(SOCKET& client, std::shared_ptr<SharedElement> socketToProcess)
	:NotInChat(socketToProcess), m_clientSocket(client) {
}


State* HaveRequestForConnection::nextState() {
	if (m_clientLeftApp) {
		return new LeftApp(m_clientSocket, this->socketToProcess);
	}
	else if (!m_response.compare("da\n")) {
	
		return new ClientInChat(m_clientSocket, this->socketToProcess);
	}
	else {
	
		return new NotConnected(m_clientSocket, this->socketToProcess);
	}
}


void  HaveRequestForConnection::messageCome(SOCKET& client, std::string& message) {

	
	m_response = message;

	if (!m_response.compare("da\n")) {
	
		socketToProcess->m_responseToProcess.enqueue(std::make_pair(1, m_clientSocket));
	}
	else {
		
		socketToProcess->m_responseToProcess.enqueue(std::make_pair(-1, m_clientSocket));
	}

	if (!message.compare("q\n")) {
		m_clientLeftApp = true;
	}
}