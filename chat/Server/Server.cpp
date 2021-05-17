#include "Server.h"
#include "OneClient.h"

#define DEFAULT_BUFLEN 512

Server::Server(std::string name, std::string port, bool &initWsa)
    :m_name(name), m_port(port) {

    m_ListenSocket = INVALID_SOCKET;
    m_clients = {};
    m_connectedClients = {};

    if (!initWSA())
        initWsa = false;
    else {

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
        initWsa = true;
    }
    

}

Server::~Server() {
    closesocket(m_ListenSocket);
    WSACleanup();
}

bool Server::initWSA() {

    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        return false;
    }

    return true;
}

void Server::run() {

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
      

       // m_clients_mutex.lock();
        m_clients.push_back(client);
       
        hookAddNewClient(m_clients[i]);

        hookTryToConnect(m_clients[i]);
   
        hookResponseToConnect(m_clients[i]);

        hookChatMessage(m_clients[i]);

        hookClientLeftTheChat(m_clients[i]);

        hookDeleteClient(m_clients[i]);
       // m_clients_mutex.unlock();
        i++;

    }

}

void Server::addNewClient(OneClient &client) {

    std::string allClients = "Trenutno povezani klijenti : ";
    bool alone = true;
   // m_clients_mutex.lock();
    for (auto c : m_clients) {
        std::string name = c->getName();
        if ((name != "") && (*c != client)) {
            allClients += name;
            allClients += "  ";
            alone = false;
        }
    }
    allClients += ".\n";

    if (alone) {
        //std::cout << "Trenutno nema povezanih klijenata" << std::endl;
        //client.sendOnlineClients("Trenutno nema povezanih klijenata\n");
    }
    else {
        client.sendOnlineClients(allClients);
        for (auto c : m_clients) {
            std::cout << c->getWaitingOtherToConnenct() << c->getName() << (*c != client) << std::endl;
           if (c->getWaitingOtherToConnenct() && c->getName() != "" && *c != client)
               c->notAlone();
        }

    } 
   // m_clients_mutex.unlock();
}

void Server::hookAddNewClient (std::shared_ptr<OneClient> client) {
    __hook(&OneClient::NewComeToChat, client.get(), &Server::addNewClient);
}

void Server::tryToConnect(OneClient &client, std::string &name) {

    int first=0, second=0;
    std::string message;
    if (!name.compare(client.getName() + "\n")) {
        message = "Ne mozete se dopisivati sami sa sobom.\nOdaberite sa kim zelite da se dopisujete.\n";
        client.sendMessageToClient(message);
    }
    else {
        bool find = false;
       // m_connectedClients_mutex.lock();
        for (auto c : m_connectedClients) {
            if (std::get<0>(c)->getSocket() == client.getSocket()) {
                find = true;
                break;
            }
            else if (std::get<1>(c)->getSocket() == client.getSocket()) {
                find = true;
                break;
            }
        }
        if(find) {
            message = "Trazeni klijent je vec zatrazio vasu dozvolu za konekciju.\n";
            client.sendMessageToClient(message);
        }
        else {
            for (unsigned i = 0; i < m_clients.size();i++) {
                if (!name.compare(m_clients[i]->getName() + "\n")) {
                    first = i;
                }
                else if (client.getSocket() == m_clients[i]->getSocket()) {
                    second = i;
                }
            }

            message = m_clients[first]->getName() + " da li zelite da se povezete sa " + client.getName() + ".\n";

            m_clients[first]->sendMessageToClient(message);
            m_connectedClients.push_back(std::make_tuple(m_clients[first], m_clients[second], 0));
        }
       // m_connectedClients_mutex.unlock();
    }

}

void Server::hookTryToConnect(std::shared_ptr<OneClient> &client) {
    __hook(&OneClient::TryToConnect, client.get(), &Server::tryToConnect);
}

void Server::hookChatMessage(std::shared_ptr<OneClient>& client) {
    __hook(&OneClient::ChatMessage, client.get(), &Server::chatMessage);
}


void Server::chatMessage(OneClient& client, std::string& message) {
   // m_connectedClients_mutex.lock();
    for (unsigned i = 0; i < m_connectedClients.size(); i++) {

        if ((std::get<0>(m_connectedClients[i]))->getSocket() == client.getSocket()) {
            (std::get<1>(m_connectedClients[i]))->sendMessageToClient(message);
                break;
        }
        else if ((std::get<1>(m_connectedClients[i]))->getSocket() == client.getSocket()) {
            (std::get<0>(m_connectedClients[i]))->sendMessageToClient(message);
            break;
        }


    }
  //  m_connectedClients_mutex.unlock();


}

void Server::responseToConnect(OneClient& client, int value) {
    //m_connectedClients_mutex.lock();
    std::string message;
    for (unsigned i = 0; i < m_connectedClients.size(); i++) {

        if ((std::get<0>(m_connectedClients[i]))->getSocket() == client.getSocket()) {
            if (value == 1) {
                //std::string message = "Klijent " + client.getName() + " zeli da se poveze sa Vama.\n";
                 message = "Uspesno ste povezani. Mozete zapoceti dopisivanje.\n";
                std::get<1>(m_connectedClients[i])->sendMessageToClient(message);
                std::get<0>(m_connectedClients[i])->sendMessageToClient(message);
                std::get<2>(m_connectedClients[i]) = 1;
                std::get<0>(m_connectedClients[i])->setCase(3);
                std::get<1>(m_connectedClients[i])->setCase(3);
                break;
            }
            else if(value==-1) {
                message = "Klijent je odbio vas zahtev za konekciju.\n";
                std::get<1>(m_connectedClients[i])->sendMessageToClient(message);
                std::get<1>(m_connectedClients[i])->setCase(1);
                std::get<0>(m_connectedClients[i])->setCase(1);
                m_connectedClients.erase(m_connectedClients.begin() + i);
                break;
            }
            else if (value == -2) {
                message = "Klijent se diskonektovao sa servera!\n";
                std::get<1>(m_connectedClients[i])->sendMessageToClient(message);
                std::get<1>(m_connectedClients[i])->setCase(1);
                //std::get<0>(m_connectedClients[i])->setCase(0);
                //std::get<1>(m_connectedClients[i])->sendMessageToClient(std::get<0>(m_connectedClients[i])->getName());
                m_connectedClients.erase(m_connectedClients.begin() + i);
                break;
            }

        }
   }
   // m_connectedClients_mutex.unlock();

   // std::cout << (std::get<0>(m_connectedClients[0]))->getSocket() << std::endl;
    //std::cout << (std::get<1>(m_connectedClients[0]))->getSocket() << std::endl;
    //std::cout << (std::get<2>(m_connectedClients[0])) << std::endl;
 
}

void Server::hookResponseToConnect(std::shared_ptr<OneClient> client) {
    __hook(&OneClient::ResponseToConnect, client.get(), &Server::responseToConnect);
}


void  Server::clientLeftTheChat(OneClient& client) {

    std::string message = client.getName() + " has left the chat\n";

    for (unsigned i = 0; i < m_connectedClients.size(); i++) {

        if ((std::get<0>(m_connectedClients[i]))->getSocket() == client.getSocket()) {
            (std::get<1>(m_connectedClients[i]))->sendMessageToClient(message);
            std::get<0>(m_connectedClients[i])->setCase(1);
            std::get<1>(m_connectedClients[i])->setCase(1);
            m_connectedClients.erase(m_connectedClients.begin() + i);
            break;
        }
        else if ((std::get<1>(m_connectedClients[i]))->getSocket() == client.getSocket()) {
            (std::get<0>(m_connectedClients[i]))->sendMessageToClient(message);
            std::get<0>(m_connectedClients[i])->setCase(1);
            std::get<1>(m_connectedClients[i])->setCase(1);
            m_connectedClients.erase(m_connectedClients.begin() + i);
            break;
        }


    }


}
void  Server::hookClientLeftTheChat(std::shared_ptr<OneClient>& client) {

    __hook(&OneClient::ClientLeftTheChat, client.get(), &Server::clientLeftTheChat);
}


void Server::deleteClient(OneClient& client) {

    for (unsigned i = 0; i < m_clients.size(); i++) {
        if (m_clients[i]->getSocket() == client.getSocket()) {
            m_clients.erase(m_clients.begin() + i);
            break;
        }
    }

}


void Server::hookDeleteClient(std::shared_ptr<OneClient>& client) {
    __hook(&OneClient::DeleteClient, client.get(), &Server::deleteClient);

}