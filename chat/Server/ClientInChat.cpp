#include "ClientInChat.h"

ClientInChat::ClientInChat(SOCKET& client, std::shared_ptr<SharedElement>& socketToProcess)
	: State(socketToProcess), m_clientSocket(client) {

};

State* ClientInChat::nextState() {
	if (m_clientLeftApp) {
		return new LeftApp(m_clientSocket, this->socketToProcess);
	}
    else if (endConversation) {

	

		Log::GetLogger()->Logg("Stanje je NotConnected");
		std::cout << "NotConnected" << std::endl;
		return new NotConnected(m_clientSocket,this->socketToProcess);
	}
	else {
		Log::GetLogger()->Logg("Stanje je ClientInChat");
		std::cout << "ClientInChat" << std::endl;
		return this;
	}
}


void  ClientInChat::messageCome(SOCKET& client, std::string& message) {

	if (!message.compare("cao\n")) {
		endConversation = true;
		std::unique_lock<std::mutex> ul(socketToProcess.get()->m_leftChatToProcess_mutex);

	
		(*socketToProcess.get()).m_leftChatSocketToProcess = client;

		socketToProcess.get()->m_leftChatToProcess_ready = true;

		ul.unlock();

		socketToProcess.get()->m_leftChatToProcess_cv.notify_one();
	}
	else if (!message.compare("q\n")) {
		m_clientLeftApp = true;
	}
	else {
		std::unique_lock<std::mutex> ul(socketToProcess.get()->m_chatMessageToProcess_mutex);

		(*socketToProcess.get()).m_chatMessageToProcess = message;
		(*socketToProcess.get()).m_chatMessageSocketToProcess = client;

		socketToProcess.get()->m_chatMessageToProcess_ready = true;

		ul.unlock();

		socketToProcess.get()->m_chatMessageToProcess_cv.notify_one();
	}
}


bool ClientInChat::isInChat() {
	return true;
}
