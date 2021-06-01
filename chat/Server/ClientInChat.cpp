#include "ClientInChat.h"

ClientInChat::ClientInChat(SOCKET& client, std::shared_ptr<SharedElement> socketToProcess)
	: State(socketToProcess), m_clientSocket(client) {

};

State* ClientInChat::nextState() {
	if (m_clientLeftApp) {
		return new LeftApp(m_clientSocket, this->socketToProcess);
	}
    else if (endConversation) {
		int val = GetCurrentThreadId();

		int bum = __LINE__;

		std::string file = __FILE__;

		std::thread::id this_id = std::this_thread::get_id();
		Log::GetLogger()->Logg("=======================");
		Log::GetLogger()->Logg(this_id);
		Log::GetLogger()->Logg("Stanje je NotConnected");
		Log::GetLogger()->Logg(bum);
		Log::GetLogger()->Logg(file);
		Log::GetLogger()->Logg("=======================");

	
		std::cout << "NotConnected" << std::endl;
		return new NotConnected(m_clientSocket,this->socketToProcess);
	}
	else {
		int val = GetCurrentThreadId();

		int bum = __LINE__;

		std::string file = __FILE__;

		std::thread::id this_id = std::this_thread::get_id();
		Log::GetLogger()->Logg("=======================");
		Log::GetLogger()->Logg(this_id);
		Log::GetLogger()->Logg("Stanje je ClientInChat");
		Log::GetLogger()->Logg(bum);
		Log::GetLogger()->Logg(file);
		Log::GetLogger()->Logg("=======================");

		std::cout << "ClientInChat" << std::endl;
		return this;
	}
}


void  ClientInChat::messageCome(SOCKET& client, std::string& message) {

	if (!message.compare("cao\n")) {
		endConversation = true;
		
		socketToProcess->m_leftChatToProcess.enqueue(client);
		
	}
	else if (!message.compare("q\n")) {
		m_clientLeftApp = true;
	}
	else {
		socketToProcess->m_chatMessageToProcess.enqueue(std::make_pair(message, client));
	
	}
}


bool ClientInChat::isInChat() {
	return true;
}
