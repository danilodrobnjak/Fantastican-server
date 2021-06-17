#include "OneClient.h"

OneClient::OneClient(SOCKET& ClientSocket, std::shared_ptr<SharedElement> socketToProcess)
    :m_ClientSocket(ClientSocket), m_socketToProcess(socketToProcess){
 
    m_read = std::make_shared<Read>(m_ClientSocket);
    m_write = std::make_shared <Write>(m_ClientSocket);

    m_state = new ClientRegistration(m_ClientSocket,&m_name, m_socketToProcess);

  
   citanje = std::thread (&Read::run, m_read);
   pisanje = std::thread(&Write::run, m_write);
   hookMessageCome(m_read);
}
OneClient::~OneClient() {

    // int iResult;
   //  iResult = shutdown(m_ClientSocket, SD_BOTH);   // Shutdown both send and receive operations.
    // if (iResult == SOCKET_ERROR) {
    //     printf("shutdown failed with error: %d\n", WSAGetLastError());
    //     closesocket(ClientSocket);
    //     WSACleanup();
    // }
    citanje.join();
    pisanje.join();
    closesocket(m_ClientSocket);
    std::cout << "Klijent destr" << std::endl;
}

std::string OneClient::getName() {
    return this->m_name;
}

SOCKET& OneClient::getSocket() {
    return this->m_ClientSocket;
}

State* OneClient::getState() {
    return m_state;
}

void OneClient::setState(State* state) {
    m_state = state;
}

bool operator!=(const OneClient& one, const OneClient& two) {
    return one.m_ClientSocket != two.m_ClientSocket;
}

void OneClient::sendMessageToClient(std::string &message) {
    m_write->messageToWrite(message);  
}

void OneClient::messageCome( std::string& message) {
    
    m_state->messageCome(m_ClientSocket,message);
    m_state = m_state->nextState(); 
  
}

void OneClient::hookMessageCome(std::shared_ptr<Read> read) {
    __hook(&Read::MessageCome,read.get(),&OneClient::messageCome);
}



