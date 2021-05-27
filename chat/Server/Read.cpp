#include "Read.h"

Read::Read(SOCKET &clientSocket)
	:m_clientSocket(clientSocket){
	m_end = false;
    ZeroMemory(m_recvbuf, m_recvbuflen);
}


void Read::run() {
    std::string message;
    while (!m_end) {
        m_iResult = recv(m_clientSocket, m_recvbuf, m_recvbuflen, 0);
        message = m_recvbuf;
        MessageCome(message);
        ZeroMemory(m_recvbuf, m_recvbuflen);
    }
}