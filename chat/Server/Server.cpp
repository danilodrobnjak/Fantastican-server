#include "Server.h"


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
 
    std::vector<std::thread> m_niti = {};


    while (true) {

        ClientSocket = accept(m_ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(m_ListenSocket);
            WSACleanup();
        }

        auto client = std::make_shared<OneClient>(ClientSocket,socketToProcess);

        m_clients_mutex.lock();
        m_niti.push_back(std::thread(&OneClient::run, client));
        m_clients.push_back(client);
        m_clients_mutex.unlock();
    }

}

void Server::addNewClient() {
   
    while (true) {

        SOCKET m_socketToProcess = socketToProcess->m_socketToProcess.dequeue();

        if (m_socketToProcess != INVALID_SOCKET) {
            std::cout << m_socketToProcess << std::endl;

            std::shared_ptr<OneClient> client = nullptr;
            m_clients_mutex.lock_shared();
            for (auto c : m_clients) {
                if (c->getSocket() == m_socketToProcess) {
                    client = c;
                    break;
                }
            }
            //odavde pocinje kopirani deo
            std::string allClients = "Trenutno povezani klijenti : ";
            bool alone = true;
            for (auto c : m_clients) {
                std::string name = c->getName();
                if ((name != "") && (c != client)) {
                    allClients += name;
                    allClients += "  ";
                    alone = false;
                }
            }
            allClients += ".\n";
            if (!alone) {
                client->sendMessageToClient(allClients);
                for (auto c : m_clients) {
                    if (!c->getState()->isInChat() && c != client) {
                        std::string newClientCome = client->getName() + " se pridruzio.\n";
                        c->sendMessageToClient(newClientCome);
                    }
                }
            }
            else {
                allClients = "Trenutno nema drugih povezanih klijenata.\n";
                client->sendMessageToClient(allClients);
            }

        }
        m_clients_mutex.unlock_shared();
    }
}


void Server::tryToConnect() {
    while(true){

            auto m_nameToProcess = socketToProcess->m_nameToProcess.dequeue();

            m_clients_mutex.lock_shared();
            std::shared_ptr<OneClient> client;

            m_clients_mutex.lock_shared();
            for (auto c : m_clients) {
                    if (c->getSocket() == m_nameToProcess.second) {
                             client = c;
                             break;
                    }
            }

           std::string name = m_nameToProcess.first;

            //kopija prethodne implementacije funkcije
           int first = 0, second = 0;
           std::string message;

           if (!name.compare(client->getName() + "\n")) {
                    message = "Ne mozete se dopisivati sami sa sobom.\nOdaberite sa kim zelite da se dopisujete.\n";
                    client->sendMessageToClient(message);
           }
           else {
                    bool find = false;
                    m_connectedClients_mutex.lock_shared();
                    for (auto c : m_connectedClients) {
                              
                             if (c.first->getSocket() == client->getSocket()) {
                                     find = true;
                                     break;
                             }
                             else if (c.second->getSocket() == client->getSocket()) {
                                     find = true;
                                     break;
                             }
                    }
                    m_connectedClients_mutex.unlock_shared();
                    if (find) {
                             message = "Trazeni klijent je vec zatrazio vasu dozvolu za konekciju.\n";
                             client->sendMessageToClient(message);
                    }
                   else {
                            for (unsigned i = 0; i < m_clients.size();i++) {
                                   if (!name.compare(m_clients[i]->getName() + "\n")) {
                                        first = i;
                                   }
                                   else if (client->getSocket() == m_clients[i]->getSocket()) {
                                        second = i;
                                   }
                            }
                            if (m_clients[first]->getState()->isInChat()) {
                                    message = "Klijent je trenutno zauzet pokusajte kasnije.\n";
                                    client->sendMessageToClient(message);
                                    client->setState(new NotConnected(client->getSocket(), socketToProcess));
                            }
                           else {
                                   message = m_clients[first]->getName() + " da li zelite da se povezete sa " + client->getName() + ".\n";

                                   m_clients[first]->sendMessageToClient(message);
                                   m_clients[first]->setState(new HaveRequestForConnection(client->getSocket(), socketToProcess));
                                   m_connectedClients_mutex.lock();
                                   m_connectedClients.push_back(std::make_pair(m_clients[first], m_clients[second]));
                                   m_connectedClients_mutex.unlock();
                           }
                   }
       
           }
           m_clients_mutex.unlock_shared();
         
    }
}


void Server::chatMessage() {
    while (true) {
       
        auto m_chatMessageToProcess = socketToProcess->m_chatMessageToProcess.dequeue();

        std::shared_ptr<OneClient> client;

        m_clients_mutex.lock_shared();
        for (auto c : m_clients) {
            if (c->getSocket() == m_chatMessageToProcess.second) {
                client = c;
                break;
            }
        }

        std::string message = m_chatMessageToProcess.first;


        //kopiranje implementacije
        m_connectedClients_mutex.lock_shared();
        for (unsigned i = 0; i < m_connectedClients.size(); i++) {

            if (m_connectedClients[i].first->getSocket() == client->getSocket()) {
                (m_connectedClients[i].second->sendMessageToClient(message));
                break;
            }
            else if (m_connectedClients[i].second->getSocket() == client->getSocket()) {
                (m_connectedClients[i].first->sendMessageToClient(message));
                break;
            }
        }
        m_connectedClients_mutex.unlock_shared();

        m_clients_mutex.unlock_shared();
        
    }
}

void Server::responseToConnect() {
    while (true) {

      auto m_responseToProcess = socketToProcess->m_responseToProcess.dequeue();

        std::shared_ptr<OneClient> client ;
        m_clients_mutex.lock_shared();
        for (auto c : m_clients) {
            if (c->getSocket() == m_responseToProcess.second) {
                client = c;
                break;
            }
        }

        int value = m_responseToProcess.first;

        //kopiram prethodnu implementaciju
        std::string message;
        m_connectedClients_mutex.lock();
        for (unsigned i = 0; i < m_connectedClients.size(); i++) {

            if ((m_connectedClients[i]).second->getSocket() == client->getSocket()) {
                if (value == 1) {
                    message = "Uspesno ste povezani. Mozete zapoceti dopisivanje.\n";
                    m_connectedClients[i].second->sendMessageToClient(message);
                    m_connectedClients[i].first->sendMessageToClient(message);
                    (m_connectedClients[i].second)->setState(new ClientInChat(client->getSocket(), socketToProcess));
                    break;
                }
                else if (value == -1) {
                    m_connectedClients[i].second->sendMessageToClient(message);
                    m_connectedClients[i].second->setState(new NotConnected(client->getSocket(), socketToProcess));
                    m_connectedClients.erase(m_connectedClients.begin() + i);
                    break;
                }

            }
        }
        m_connectedClients_mutex.unlock();

        m_clients_mutex.unlock_shared();
     
    }
}

void  Server::clientLeftTheChat() {
    while (true) {
        
        auto m_leftChatToProcess = socketToProcess->m_leftChatToProcess.dequeue();

        std::shared_ptr<OneClient> client ;
        m_clients_mutex.lock_shared();
        for (auto c : m_clients) {
            if (c->getSocket() == m_leftChatToProcess) {
                client = c;
                break;
            }
        }

        //kopiram prethodnu implementaciju
        std::string message = client->getName() + " has left the chat\n";
        m_connectedClients_mutex.lock();
        for (unsigned i = 0; i < m_connectedClients.size(); i++) {

            if ((m_connectedClients[i].first)->getSocket() == client->getSocket()) {
                (m_connectedClients[i].second)->sendMessageToClient(message);
                m_connectedClients[i].second->setState(new NotConnected(client->getSocket(), socketToProcess));
                m_connectedClients.erase(m_connectedClients.begin() + i);
                break;
            }
            else if ((m_connectedClients[i].second)->getSocket() == client->getSocket()) {
                (m_connectedClients[i].first)->sendMessageToClient(message);
                m_connectedClients[i].first->setState(new NotConnected(client->getSocket(), socketToProcess));
                m_connectedClients.erase(m_connectedClients.begin() + i);
                break;
            }


        }
        m_connectedClients_mutex.unlock();

        m_clients_mutex.unlock_shared();
     
    }
}


void Server::deleteClient(){
    while (true) {
      
        auto m_clientLeftAppToProcess = socketToProcess->m_clientLeftAppToProcess.dequeue();

        std::shared_ptr<OneClient> client;
        m_clients_mutex.lock();

        for (auto c : m_clients) {
            if (c->getSocket() == m_clientLeftAppToProcess) {
                client = c;
                break;
            }
        }
        //kopija implementacije
        
        for (unsigned i = 0; i < m_clients.size(); i++) {
            if (m_clients[i]->getSocket() == client->getSocket()) {
                m_clients.erase(m_clients.begin() + i);
                break;
            }
        }
        m_clients_mutex.unlock();


        

    }
}
