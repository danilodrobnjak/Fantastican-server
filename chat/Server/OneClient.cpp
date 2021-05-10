#include "OneClient.h"
#include <cstdio>


OneClient::OneClient(SOCKET& ClientSocket)
    :m_ClientSocket(ClientSocket) {

    m_waitingOtherToConnenct = true;
    ZeroMemory(m_recvbuf, m_recvbuflen);
    ZeroMemory(m_sendbuf, m_sendbuflen);

    m_read = std::make_shared<Read>(m_ClientSocket);
    m_write = std::make_shared <Write>(m_ClientSocket);

   // hookmessageToWrite(this);

 //   std::thread citanje(&Read::run, m_read);
  //  std::thread pisanje(&Write::run, m_write);


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


    //1st case
    int iResult;

    std::thread citanje(&Read::run, m_read);
    std::thread pisanje(&Write::run, m_write);

    hookMessageCome(m_read);
    hookmessageToWrite(this);


    while (m_case == 0);
   // iResult = recv(m_ClientSocket, m_recvbuf, m_recvbuflen, 0);
   // m_name = m_recvbuf;
    //std::cout << "Client name" << m_name << std::endl;
   // ZeroMemory(m_recvbuf, m_recvbuflen);
 
    NewComeToChat(*this);
    while (m_waitingOtherToConnenct);
    NewComeToChat(*this);

    //2nd case
    std::string odgovor = "Odaberite s kim zelite da se dopisujete\n";

    //iResult = send(m_ClientSocket, odgovor.c_str(), (int)strlen(odgovor.c_str()), 0);

    //ZeroMemory(m_recvbuf, m_recvbuflen);

    MessageToWrite(odgovor);


    while (true);



    iResult = recv(m_ClientSocket, m_recvbuf, m_recvbuflen, 0);


   odgovor = m_recvbuf;
     
    TryToConnect(*this,odgovor);

    ZeroMemory(m_recvbuf, m_recvbuflen);


    iResult = recv(m_ClientSocket, m_recvbuf, m_recvbuflen, 0);

    std::cout << this->m_name << m_recvbuf;

    odgovor = m_recvbuf;

    if (!odgovor.compare("da\n"))
        ResponseToConnect(*this,1);
    else
    {
        ResponseToConnect(*this, -1);
    }

    ZeroMemory(m_recvbuf, m_recvbuflen);
   //posaljemo odg da li zeli

   // odgovor = "Slaze se";

   // iResult = send(ClientSocket, odgovor.c_str(), (int)strlen(odgovor.c_str()), 0);

   // ZeroMemory(recvbuf, sizeof(recvbuf));
   // std::thread citanje(&Read::run, m_read);
   // std::thread pisanje(&Write::run, m_write);

   // hookMessageCome(m_read);


    //3rd case
    while (true) {


        iResult = recv(m_ClientSocket, m_recvbuf, m_recvbuflen, 0);
    
        if (!strcmp(m_recvbuf, "cao\n"))
            break;

        std::cout << m_recvbuf;

        iResult = send(m_ClientSocket, m_recvbuf, (int)strlen(m_recvbuf), 0);


        ZeroMemory(m_recvbuf, m_recvbuflen);

    }

    citanje.join();
    pisanje.join();


}



std::string OneClient::getName() {
    return this->m_name;
}

SOCKET OneClient::getSocket() {
    return this->m_ClientSocket;
}


void OneClient::sendOnlineClients(std::string message) {

    if(message != "Trenutno nema povezanih klijenata\n")
        m_waitingOtherToConnenct = false;

    send(m_ClientSocket, message.c_str(), (int)strlen(message.c_str()), 0);

}


bool operator!=(const OneClient &one,const OneClient &two) {

    return one.m_ClientSocket != two.m_ClientSocket;

}

void OneClient::notAlone(){
    m_waitingOtherToConnenct = false;
    NewComeToChat(*this);
}


bool OneClient::getWaitingOtherToConnenct() {
    return m_waitingOtherToConnenct;
}

void OneClient::messageCome(const std::string& message) {
    std::cout << "Client send: " << message;
    if (m_case == 0) {
        m_name = message;
        m_case = 1;
    }
}

void OneClient::hookMessageCome(std::shared_ptr<Read> read) {

  
    __hook(&Read::MessageCome,read.get(),&OneClient::messageCome);
}

void OneClient::hookmessageToWrite(OneClient* write) {

    __hook(&OneClient::MessageToWrite, write, &Write::messageToWrite);
}
