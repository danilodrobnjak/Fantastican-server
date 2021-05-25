#include "HaveRequestForConnection.h"
HaveRequestForConnection::HaveRequestForConnection(SOCKET& client, std::shared_ptr<SharedElement>& socketToProcess)
	:NotInChat(socketToProcess), m_clientSocket(client) {
}


State* HaveRequestForConnection::nextState() {
	if (m_clientLeftApp) {
		return new LeftApp(m_clientSocket, this->socketToProcess);
	}
	if (!m_response.compare("da\n")) {
	
		return new ClientInChat(m_clientSocket, this->socketToProcess);
	}
	else {
	
		return new NotConnected(m_clientSocket, this->socketToProcess);
	}
}


void  HaveRequestForConnection::messageCome(SOCKET& client, std::string& message) {

	//std::cout << " HaveRequestForConnection " << message << std::endl;

	m_response = message;

	if (!m_response.compare("da\n")) {
		
		std::unique_lock<std::mutex> ul(socketToProcess.get()->m_responseToProcess_mutex);

		(*socketToProcess.get()).m_responseToProcess = 1;
		(*socketToProcess.get()).m_responseSocketToProcess = m_clientSocket;

		socketToProcess.get()->m_responseToProcess_ready = true;

		ul.unlock();

		socketToProcess.get()->m_responseToProcess_cv.notify_one();

	}
	else {
		
		std::unique_lock<std::mutex> ul(socketToProcess.get()->m_responseToProcess_mutex);

		(*socketToProcess.get()).m_responseToProcess = -1;
		(*socketToProcess.get()).m_responseSocketToProcess = m_clientSocket;

		socketToProcess.get()->m_responseToProcess_ready = true;

		ul.unlock();

		socketToProcess.get()->m_responseToProcess_cv.notify_one();
		
	}

	if (!message.compare("q\n")) {
		m_clientLeftApp = true;
	}
}