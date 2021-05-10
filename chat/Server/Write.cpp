#include "Write.h"

Write::Write(SOCKET& clientSocket)
    :m_clientSocket(clientSocket) {
    m_end = false;
    ZeroMemory(m_sendbuf, m_sendbuflen);
}


void Write::run() {
    std::cout << "usao u run write" << std::endl;
    while (true) {
        while (m_newMessage == false);
       // while (m_newMessage == true && m_end == false) {
            std::cout << "usao u write" << std::endl;
            m_iResult = send(m_clientSocket, m_sendbuf, m_sendbuflen, 0);
            m_newMessage = false;
            if (!strcmp(m_sendbuf, "cao\n"))
                m_end = true;
            ZeroMemory(m_sendbuf, m_sendbuflen);
        //}
    }
}


void Write::messageToWrite(const std::string& message) {
    m_newMessage = true;
    strcpy(m_sendbuf,message.c_str());
    std::cout << m_newMessage;
    std::cout <<m_sendbuf;
}

//void Write::hookmessageToWrite(OneClient *write) {

 //   __hook(&OneClient::MessageToWrite,write,&Write::messageToWrite);
//}
