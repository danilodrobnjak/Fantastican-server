
#include "WaitForResponseToConnect.h"

WaitForResponseToConnect::WaitForResponseToConnect(SOCKET& client, std::shared_ptr<SharedElement>& socketToProcess)
	:NotInChat(socketToProcess), m_clientSocket(client) {
}


State* WaitForResponseToConnect::nextState() {
	std::cout << " WaitForResponseToConnect " << std::endl;
	int val = GetCurrentThreadId();

	int bum = __LINE__;

	std::string file = __FILE__;

	std::thread::id this_id = std::this_thread::get_id();
	Log::GetLogger()->Logg("=======================");
	Log::GetLogger()->Logg(this_id);
	Log::GetLogger()->Logg("Stanje je WaitForResponseToConnect");
	Log::GetLogger()->Logg(bum);
	Log::GetLogger()->Logg(file);
	Log::GetLogger()->Logg("=======================");
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