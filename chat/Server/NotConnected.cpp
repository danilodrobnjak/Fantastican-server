#include "NotConnected.h"
#include "WaitForResponseToConnect.h"


State* NotConnected::nextState() {
	std::cout << "WaitForResponseToConnect " << std::endl;
	return new WaitForResponseToConnect(m_clientSocket);
}


void NotConnected::messageCome(SOCKET& client,std::string& message) {

	//std::cout << "NotConnected " << message << std::endl;

}


NotConnected::NotConnected(SOCKET& client) {
	m_clientSocket = client;
	// daj mi listu klijenata
	//postavimo varijablu da otkoci nit koja to radi

}