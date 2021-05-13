#include "OneClient.h"
#include <cstdio>


OneClient::OneClient(SOCKET& ClientSocket)
    :m_ClientSocket(ClientSocket) {

    m_waitingOtherToConnenct = true;
    ZeroMemory(m_recvbuf, m_recvbuflen);
    ZeroMemory(m_sendbuf, m_sendbuflen);

    m_read = std::make_shared<Read>(m_ClientSocket);
    m_write = std::make_shared <Write>(m_ClientSocket);

}
OneClient::~OneClient() {

    // int iResult;
    // iResult = shutdown(ClientSocket, SD_BOTH);   // Shutdown both send and receive operations.
    // if (iResult == SOCKET_ERROR) {
    //     printf("shutdown failed with error: %d\n", WSAGetLastError());
    //     closesocket(ClientSocket);
    //     WSACleanup();
    // }

    closesocket(m_ClientSocket);
    std::cout << "Klijent destr" << std::endl;
}

void OneClient::run() {


    std::thread citanje(&Read::run, m_read);
    std::thread pisanje(&Write::run, m_write);

    hookMessageCome(m_read);

    while (m_case == 0);
  
    NewComeToChat(*this);
    while (m_waitingOtherToConnenct);

    while (m_case == 1);
  
    m_write->messageToWrite("Odaberite s kim zelite da se dopisujete\n");
   

    citanje.join();
    pisanje.join();


}



std::string OneClient::getName() {
    return this->m_name;
}

SOCKET OneClient::getSocket() {
    return this->m_ClientSocket;
}

void OneClient::setCase(int nCase) {
    m_case = nCase;
}


void OneClient::sendOnlineClients(std::string message) {

    if(message != "Trenutno nema povezanih klijenata\n")
        m_waitingOtherToConnenct = false;
    m_write->messageToWrite(message);
    m_case = 2;
}
void OneClient::sendMessageToClient(std::string &message) {
  
    m_write->messageToWrite(message);
   
}

bool operator!=(const OneClient &one,const OneClient &two) {

    return one.m_ClientSocket != two.m_ClientSocket;

}

void OneClient::notAlone(){
    NewComeToChat(*this);
}


bool OneClient::getWaitingOtherToConnenct() {
    return m_waitingOtherToConnenct;
}

void OneClient::messageCome( std::string& message) {
    std::cout << "Client send: " << message  << m_case<< std::endl;
    
    if (m_case == 0) {
        m_name = message;
        m_case = 1;
    }
    else if (m_case == 2) {
        if (message.compare("da\n") != 0) {
            TryToConnect(*this, message);
        }
        else {
            ResponseToConnect(*this, 1);
        }
    }
    else if (m_case == 3) {
        ChatMessage(*this, message);
    }
}

void OneClient::hookMessageCome(std::shared_ptr<Read> read) {
    __hook(&Read::MessageCome,read.get(),&OneClient::messageCome);
}



