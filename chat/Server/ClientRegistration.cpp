#include "ClientRegistration.h"



ClientRegistration::ClientRegistration(SOCKET& client,std::string *name, std::shared_ptr<SharedElement>& socketToProcess)
	:State(socketToProcess),m_name(name),m_clientSocket(client){

}


State* ClientRegistration::nextState() {
	int val = GetCurrentThreadId();

	int bum= __LINE__;

	std::string file = __FILE__;

	std::thread::id this_id = std::this_thread::get_id();
	
	Log::GetLogger()->Logg("=======================");
	Log::GetLogger()->Logg(this_id);
	Log::GetLogger()->Logg("Stanje je NotConnected");
	Log::GetLogger()->Logg(bum);
	Log::GetLogger()->Logg(file);
	Log::GetLogger()->Logg("=======================");
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