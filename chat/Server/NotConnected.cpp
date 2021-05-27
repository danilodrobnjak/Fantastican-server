#include "NotConnected.h"


State* NotConnected::nextState() {
	
	std::cout << "WaitForResponseToConnect " << std::endl;
	Log::GetLogger()->Logg("Stanje je WaitForResponseToConnect");
	if (m_clientLeftApp) {
		return new LeftApp(m_clientSocket, this->socketToProcess);
	}
	else {
		return new WaitForResponseToConnect(m_clientSocket, this->socketToProcess);
	}
}


void NotConnected::messageCome(SOCKET& client,std::string& message) {

	//std::cout << "NotConnected " << message << std::endl;
	std::unique_lock<std::mutex> ul(socketToProcess.get()->m_nameToProcess_mutex);

	(*socketToProcess.get()).m_nameToProcess = message;
	(*socketToProcess.get()).m_nameSocketToProcess = client;

	socketToProcess.get()->m_nameToProcess_ready = true;

	ul.unlock();

	socketToProcess.get()->m_nameToProcess_cv.notify_one();

	if (!message.compare("q\n")) {
		m_clientLeftApp = true;
	}

}


NotConnected::NotConnected(SOCKET& client, std::shared_ptr<SharedElement>& socketToProcess)
:NotInChat(socketToProcess){
	m_clientSocket = client;

	//std::unique_lock<std::mutex> lk(getOnlineClients_mutex);
	//getOnlineClients_cv.wait(lk);
	
	std::unique_lock<std::mutex> ul(socketToProcess.get()->m_socketToProcess_mutex);

	 (*socketToProcess.get()).m_socketToProcess = m_clientSocket;

	 socketToProcess.get()->m_socketToProcess_ready = true;

	 ul.unlock();

	 socketToProcess.get()->m_socketToProcess_cv.notify_one();


	

	//std::cout << *socketToProcess.get() << "ispisi ga sve mu jebem\n" << std::endl;
	
	//lk.unlock();.30
	// 
	//getOnlineClients_cv.notify_one();

	// daj mi listu klijenata
	//postavimo varijablu da otkoci nit koja to radi

}