#include "Write.h"

Write::Write(SOCKET& clientSocket)
    :m_clientSocket(clientSocket) {
    m_end = false;
    ZeroMemory(m_sendbuf, m_sendbuflen);
}


void Write::run() {
    while (true) {
        while (true) {
            mtx.lock();
            if (m_newMessage != false) {
                mtx.unlock();
                break;
            }
            mtx.unlock();
        }
            mtx.lock();
            m_iResult = send(m_clientSocket, m_sendbuf, m_sendbuflen, 0);
            if ((m_iResult == 0))
                break;
            //std::cout << m_iResult;
           // std::cout << m_sendbuf << std::endl;
            m_newMessage = false;
            if (!strcmp(m_sendbuf, "cao\n"))
                m_end = true;
            ZeroMemory(m_sendbuf, m_sendbuflen);
            mtx.unlock();

    }
}


void Write::messageToWrite(const std::string& message) {
    mtx.lock();
    m_newMessage = true;
    strcpy(m_sendbuf,message.c_str());
    mtx.unlock();
}

