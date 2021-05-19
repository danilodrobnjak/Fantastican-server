#include "OneClient.h"
#include <cstdio>


OneClient::OneClient(SOCKET& ClientSocket)
    :m_ClientSocket(ClientSocket) {

    m_waitingOtherToConnenct = true;
    ZeroMemory(m_recvbuf, m_recvbuflen);
    ZeroMemory(m_sendbuf, m_sendbuflen);

    m_read = std::make_shared<Read>(m_ClientSocket);
    m_write = std::make_shared <Write>(m_ClientSocket);

    m_state = new ClientRegistration(m_ClientSocket,&m_name);
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
   

    while (true) {
    
        NewComeToChat(*this);
        while (m_waitingOtherToConnenct);
        

        while (m_case == 1);
      //  m_write->messageToWrite("Odaberite s kim zelite da se dopisujete\n");


        while (m_case == 2 || m_case == 3);
      
    }
 

    citanje.join();
    pisanje.join();


}



std::string OneClient::getName() {
    return this->m_name;
}

SOCKET& OneClient::getSocket() {
    return this->m_ClientSocket;
}

void OneClient::setCase(int nCase) {
    m_case = nCase;
}


void OneClient::sendOnlineClients(std::string message) {

   // if(message != "Trenutno nema povezanih klijenata\n")
   //     m_waitingOtherToConnenct = false;
    m_write->messageToWrite(message);
    m_case = 2;
    //m_write->messageToWrite("Odaberite s kim zelite da se dopisujete\n");
}
void OneClient::sendMessageToClient(std::string &message) {
  
    m_write->messageToWrite(message);
   
}

bool operator!=(const OneClient &one,const OneClient &two) {

    return one.m_ClientSocket != two.m_ClientSocket;

}

void OneClient::notAlone(){
   //NewComeToChat(*this);
}


bool OneClient::getWaitingOtherToConnenct() {
    return m_waitingOtherToConnenct;
}

void OneClient::messageCome( std::string& message) {
  //  std::cout << "Client send: " << message  << m_case<< std::endl;
    
    m_state->messageCome(m_ClientSocket,message);
    m_state = m_state->nextState(); 

    if (message.compare("q\n") == 0) {
  
        if(m_case == 2)
            ResponseToConnect(*this, -2);
        if(m_case == 3)
            ClientLeftTheChat(*this);

        DeleteClient(*this);
        return;
    }
    else {
        if (m_case == 0) {
        //    m_name = message;
            m_case = 1;
        }
        else if (m_case == 2) {
            if (message.compare("da\n") != 0 && message.compare("ne\n") != 0) {
                TryToConnect(*this, message);
            }
            else if (message.compare("da\n") == 0){
                ResponseToConnect(*this, 1);
            }
            else if (message.compare("ne\n") == 0) {
                ResponseToConnect(*this, -1);
            }
        }
        else if (m_case == 3) {
            if (message.compare("cao\n") == 0)
                ClientLeftTheChat(*this);
            else {
                ChatMessage(*this, message);
            }
        }
    }
   
}

void OneClient::hookMessageCome(std::shared_ptr<Read> read) {
    __hook(&Read::MessageCome,read.get(),&OneClient::messageCome);
}



