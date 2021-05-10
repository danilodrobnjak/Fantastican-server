#include "Server.h"
#include "OneClient.h"

#define DEFAULT_BUFLEN 512

Server::Server(std::string name, std::string port)
    :m_name(name), m_port(port) {

    m_ListenSocket = INVALID_SOCKET;
    m_clients = {};
    //m_niti = {};
    m_connectedClients = {};
    m_requestedConnections = {};

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
    }

    // Create a SOCKET for connecting to server
    m_ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (m_ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
    }

    // Setup the TCP listening socket
    iResult = bind(m_ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(m_ListenSocket);
        WSACleanup();
    }

    freeaddrinfo(result);

    iResult = listen(m_ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(m_ListenSocket);
        WSACleanup();
    }

}

Server::~Server() {
    closesocket(m_ListenSocket);
    WSACleanup();
}

void Server::initWSA() {

    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
    }
}

void Server::run() {

    int iResult;
    SOCKET ClientSocket = INVALID_SOCKET;
    int i = 0;

    std::vector<std::thread> m_niti = {};

    while (true) {

        ClientSocket = accept(m_ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(m_ListenSocket);
            WSACleanup();
        }

        auto client = std::make_shared<OneClient>(ClientSocket);

        m_niti.push_back(std::thread(&OneClient::run, client));

        m_clients.push_back(client);

        hookAddNewClient(m_clients[i]);

        hookTryToConnect(m_clients[i]);
   
        hookResponseToConnect(m_clients[i]);

        i++;

    }

}

void Server::addNewClient(OneClient &client) {

    std::string allClients = "Trenutno povezani klijenti : ";
    bool alone = true;
    for (auto c : m_clients) {
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
        client.sendOnlineClients(allClients);
        for (auto c : m_clients) {
           if (c->getWaitingOtherToConnenct() && c->getName() != "" && *c != client)
                c->notAlone();
        }

    } 
}

void Server::hookAddNewClient (std::shared_ptr<OneClient> client) {
    __hook(&OneClient::NewComeToChat, client.get(), &Server::addNewClient);
}

void Server::tryToConnect(OneClient& client, std::string &name) {
    for (auto c : m_clients) {
        if (!name.compare(c->getName() + "\n")) {
            std::string message = c->getName() + " da li zelite da se povezete sa " + client.getName();
            send(c->getSocket(), message.c_str(), (int)strlen(message.c_str()), 0);
            m_requestedConnections[c->getSocket()].push_back(std::make_pair(client.getSocket(), 0));
            break;
        }
    }
}

void Server::hookTryToConnect(std::shared_ptr<OneClient> &client) {
    __hook(&OneClient::TryToConnect, client.get(), &Server::tryToConnect);
}



void Server::responseToConnect(OneClient& client, int value) {

  
    for (auto x : m_connectedClients) {
        std::cout << x.first << "   " << x.second  <<std::endl;
    }
 
    auto elem = m_requestedConnections[client.getSocket()].begin()->first;

    if (value == 1) {
        std::string message = "Klijent " + client.getName() + " zeli da se poveze sa Vama";
        send(elem, message.c_str(), (int)strlen(message.c_str()), 0);
        this->m_connectedClients.push_back(std::make_pair(client.getSocket(),elem));
        //requestedConnections[client.getName()].erase(requestedConnections[client.getSocket()].begin());
    }
    if (value == -1) {
        std::string message = "Klijent " + client.getName() + "ne zeli da se poveze sa Vama";
        send(elem, message.c_str(), (int)strlen(message.c_str()), 0);
      //  requestedConnections[client.getName()].pop_back();
    }

}

void Server::hookResponseToConnect(std::shared_ptr<OneClient> client) {
    __hook(&OneClient::ResponseToConnect, client.get(), &Server::responseToConnect);
}

