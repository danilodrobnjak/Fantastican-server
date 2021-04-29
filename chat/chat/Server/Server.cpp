#include "Server.h"
#include "OneClient.h"

#define DEFAULT_BUFLEN 512

Server::Server(std::string name, std::string port)
    :name(name), port(port) {

    ListenSocket = INVALID_SOCKET;
    clients = {};

    initWSA();

    int iResult;
    struct addrinfo* result = NULL;
    struct addrinfo hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;            // The Internet Protocol version 4 (IPv4) address family.
    hints.ai_socktype = SOCK_STREAM;      // Provides sequenced, reliable, two-way, connection-based byte streams with an OOB data transmission mechanism. (TCP)
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;          //The socket address will be used in a call to the bindfunction.

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, port.c_str(), &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        //return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        // return 1;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        // return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        // return 1;
    }

}

Server::~Server() {
    closesocket(ListenSocket);
    WSACleanup();
}

void Server::initWSA() {

    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        //return 1;
    }
}

void Server::run() {

    int iResult;
    SOCKET ClientSocket = INVALID_SOCKET;
    int i = 0;

    std::vector<std::thread> niti = {};

    while (true) {

        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            // return 1;
        }

        auto client = std::make_shared<OneClient>(ClientSocket);

        niti.push_back(std::thread(&OneClient::run, client));

        clients.push_back(client);

        hookAddNewClient(clients[i]);

        hookTryToConnect(clients[i]);
   
        i++;

    }

}

void Server::addNewClient(OneClient &client) {

    std::string allClients = "Trenutno povezani klijenti : ";
    bool alone = true;
    for (auto c : clients) {
        std::string name = c->getName();
        if ((name != "") && (*c != client)) {
            allClients += name;
            allClients += "  ";
            alone = false;
        }
    }
    allClients += "\n";

    if (alone) {
        std::cout << "Trenutno nema povezanih klijenata" << std::endl;
        client.sendOnlineClients("Trenutno nema povezanih klijenata\n");
    }
    else {
        std::cout << allClients << std::endl;
        //client.waitingOtherToConnenct = false;
        client.sendOnlineClients(allClients);
        //client.waitingOtherToConnenct = false;
        for (auto c : clients) {
           if (c->waitingOtherToConnenct && c->getName() != "" && *c != client)
               //c->waitingOtherToConnenct = false;
                c->notAlone();
        }

    } 
}

void Server::hookAddNewClient (std::shared_ptr<OneClient> client) {
    __hook(&OneClient::NewComeToChat, client.get(), &Server::addNewClient);
}

void Server::tryToConnect(OneClient& client,const std::string &name) {

    for (auto c : clients) {
        if (!std::strcmp(c->getName().c_str(), name.c_str())) {
            std::string message = c->getName() + " da li zelite da se povezete sa " + name;
            send(c->getSocket(), message.c_str(), (int)strlen(message.c_str()), 0);
        }
    }
}

void Server::hookTryToConnect(std::shared_ptr<OneClient> client) {
    __hook(&OneClient::tryToConnect, client.get(), &Server::tryToConnect);
}

