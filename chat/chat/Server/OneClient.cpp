#include "OneClient.h"
#include <cstdio>


OneClient::OneClient(SOCKET& ClientSocket)
    :ClientSocket(ClientSocket) {

    waitingOtherToConnenct = true;
    ZeroMemory(recvbuf, sizeof(recvbuf));
    ZeroMemory(sendbuf, sizeof(sendbuf));
}
OneClient::~OneClient() {

    // int iResult;
    // iResult = shutdown(ClientSocket, SD_BOTH);   // Shutdown both send and receive operations.
    // if (iResult == SOCKET_ERROR) {
    //     printf("shutdown failed with error: %d\n", WSAGetLastError());
    //     closesocket(ClientSocket);
    //     WSACleanup();
    // }

    closesocket(ClientSocket);
    std::cout << "Klijent destr" << std::endl;
}

void OneClient::run() {

    int iResult;
    //char recvbuf[512];
    //int recvbuflen = 512;

    iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
    name = recvbuf;
    std::cout << "Client name " << name << std::endl;
    ZeroMemory(recvbuf, sizeof(recvbuf));
 
    NewComeToChat(*this);

    while (waitingOtherToConnenct) {
      //  NewComeToChat(*this);
    }

    NewComeToChat(*this);

    std::string odgovor = "Odaberite s kim zelite da se dopisujete";

    iResult = send(ClientSocket, odgovor.c_str(), (int)strlen(odgovor.c_str()), 0);

    ZeroMemory(recvbuf, sizeof(recvbuf));

    iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);

    std::cout << name << " zeli da se poveze sa : " << recvbuf ;

    //pitaj ovog jel prihvata cet 
    ZeroMemory(recvbuf, sizeof(recvbuf));


    //posaljemo odg da li zeli

   // odgovor = "Slaze se";

   // iResult = send(ClientSocket, odgovor.c_str(), (int)strlen(odgovor.c_str()), 0);

   // ZeroMemory(recvbuf, sizeof(recvbuf));


    while (true) {


        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
    
        if (!strcmp(recvbuf, "cao\n"))
            break;

        std::cout << recvbuf;

        iResult = send(ClientSocket, recvbuf, (int)strlen(recvbuf), 0);


        ZeroMemory(recvbuf, sizeof(recvbuf));

    }
   // std::thread citanje(&read, this->ClientSocket);
    //std::thread pisanje(&write, this->ClientSocket);
    //pisanje.join();
   // citanje.join();

}



std::string OneClient::getName() {
    return this->name;
}

SOCKET OneClient::getSocket() {
    return this->ClientSocket;
}


void OneClient::sendOnlineClients(std::string message) {

    if(message != "Trenutno nema povezanih klijenata\n")
        waitingOtherToConnenct = false;

    send(ClientSocket, message.c_str(), (int)strlen(message.c_str()), 0);

   // if (message != "Trenutno nema povezanih klijenata\n")
    //    waitingOtherToConnenct = false;
}


bool operator!=(const OneClient &one,const OneClient &two) {

    return one.ClientSocket != two.ClientSocket;

}

void OneClient::notAlone(){
    waitingOtherToConnenct = false;
    NewComeToChat(*this);
}


void OneClient::read(const SOCKET ConnectSocket) {

    int iResult;
    while (!end) {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        std::cout << recvbuf << std::endl;
        ZeroMemory(recvbuf, sizeof(recvbuf));
    }

}

void OneClient::write(const SOCKET ConnectSocket) {
    while (getchar() != '\n');
    int iResult;
    while (!end) {

        fgets(sendbuf, sendbuflen, stdin);
        iResult = send(ConnectSocket, sendbuf, sendbuflen, 0);
        if (!strcmp(sendbuf, "cao\n"))
            end = true;
        ZeroMemory(sendbuf, sizeof(sendbuf));
    }
}