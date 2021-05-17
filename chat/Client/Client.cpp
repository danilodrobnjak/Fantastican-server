#include "Client.h"


Client::Client(){

    ZeroMemory(name, sizeof(name));
    
    struct addrinfo* result = NULL, // A pointer to a linked list of one or more addrinfo structures that contains response information about the host.
        * ptr = NULL,
        hints;   // addrinfo structure that provides hints about the type of socket the caller supports.
  

      // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) throw E_WSAStartup();

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo("localhost", DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        WSACleanup();
        throw E_ADDR_INFO();
       
    }

    for (ptr = result; ptr != NULL;ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            WSACleanup();
            throw E_Socket();
           
        }

        // Connect to server.
        //(ovde sad pokusava konekciju na konkretan server svojim soketom koji je napravio gore)
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    // ako na kraju svih mogucih ip adresa za taj "localhost" ovo ostane prazno prekidamo jer nismo uspeli da se povezemo
    if (ConnectSocket == INVALID_SOCKET) {
        WSACleanup();
        throw E_Server();

    }


}



Client::~Client(){
 
    iResult = shutdown(ConnectSocket, SD_BOTH);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
    }

    closesocket(ConnectSocket);
    WSACleanup();

}


void Client::read() {

    int iResult;
    while (!end) {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
        std::cout << recvbuf;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);


        ZeroMemory(recvbuf, sizeof(recvbuf));
    }

}

void Client::write() {
    // while (getchar() != '\n');
    int iResult;
    while (!end) {
        // while (getchar() != '\n');
        //std::cout << " [ " << name << " ] : ";
        fgets(sendbuf, sendbuflen, stdin);
        iResult = send(ConnectSocket, sendbuf, sendbuflen, 0);
        if (!strcmp(sendbuf, "q\n")) {
            end = true;
        }
        ZeroMemory(sendbuf, sizeof(sendbuf));
    }
}


void Client::run() {

    printf("Enter your name first : ");

    std::cin >> name;

    while (getchar() != '\n');

    iResult = send(ConnectSocket, name, (int)strlen(name), 0);

    std::cout << "Sacekajte da proverimo ko je online .....\n";

    std::thread citanje(&Client::read,*this);
    std::thread pisanje(&Client::write, *this);

    citanje.join();
    pisanje.join();


}