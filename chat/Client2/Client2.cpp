#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//                CLIENT
/// ////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////

char sendbuf[DEFAULT_BUFLEN];
char recvbuf[DEFAULT_BUFLEN];
int recvbuflen = DEFAULT_BUFLEN;
int sendbuflen = DEFAULT_BUFLEN;
bool end = false;
char name[512];


void read(const SOCKET ConnectSocket) {

    int iResult;
    while (!end) {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
        std::cout<<  recvbuf ;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
        ZeroMemory(recvbuf, sizeof(recvbuf));
    }

}

void write(const SOCKET ConnectSocket) {
    // while (getchar() != '\n');
    int iResult;
    while (!end) {
        // while (getchar() != '\n');
        //std::cout << " [ " << name << " ] : ";
        fgets(sendbuf, sendbuflen, stdin);
        iResult = send(ConnectSocket, sendbuf, sendbuflen, 0);
        if (!strcmp(sendbuf, "cao\n"))
            end = true;
        ZeroMemory(sendbuf, sizeof(sendbuf));
    }
}



int __cdecl main(int argc, char** argv)
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL, // A pointer to a linked list of one or more addrinfo structures that contains response information about the host.
        * ptr = NULL,
        hints;   // addrinfo structure that provides hints about the type of socket the caller supports.
   // char sendbuf[DEFAULT_BUFLEN];
    //std::string sendbuf = "";
    //std::string recvbuf = "";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;                       //br procitanih bajtova
    int recvbuflen = DEFAULT_BUFLEN;
    //  int sendbuflen = DEFAULT_BUFLEN;



      // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo("localhost", DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(5));


    // Attempt to connect to an address until one succeeds 
    //(ovde on ide kroz listu ip adresa za tu trazenu "localhost" pa ako ih ima vise pokusava da se poveze dok ne uspe na neku)
    for (ptr = result; ptr != NULL;ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
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
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }


    // ZeroMemory(sendbuf, sizeof(sendbuf));
    // ZeroMemory(recvbuf, sizeof(recvbuf));


     //klijent prvo salje svoje ime da bi mogao da cetuje
    printf("Enter your name first : ");

   // char name[512];
    ZeroMemory(name, sizeof(name));
    std::cin >> name;

    while (getchar() != '\n');

    iResult = send(ConnectSocket, name, (int)strlen(name), 0);

    //  iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);

    std::cout << "Sacekajte da proverimo ko je online .....\n";

    //  while (true) {
    //      ZeroMemory(recvbuf, sizeof(recvbuf));
     //     iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
    //      if (strcmp(recvbuf, "Trenutno nema povezanih klijenata\n"))
     //         break;

     // }


    std::thread citanje(&read, ConnectSocket);
    std::thread pisanje(&write, ConnectSocket);

    citanje.join();
    pisanje.join();



    //std::cout << recvbuf;
    //ZeroMemory(recvbuf, sizeof(recvbuf));

    //posalje s kim hoce da se dop.
    //std::cout << "Molimo unesite s kim hovete da se dop...." << std::endl;
   // fgets(recvbuf, recvbuflen, stdin);
   // iResult = send(ConnectSocket, recvbuf, (int)strlen(recvbuf), 0);

   // std::cout << "Molimo sacekajte...." << std::endl;

   // ZeroMemory(recvbuf, sizeof(recvbuf));

    //iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);

    ///std::cout << recvbuf;

    //std::cout << recvbuf << std::endl;

    //ZeroMemory(sendbuf, sizeof(sendbuf));
    //ZeroMemory(recvbuf, sizeof(recvbuf));
    //neka bude u igti dok ne kaze cao
    //while (true) {

      //  printf("[ %s ] : ", name);


      //  fgets(sendbuf, sendbuflen, stdin);



      //  iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);

      //  if (iResult == SOCKET_ERROR) {
       ///     printf("send failed with error: %d\n", WSAGetLastError());
       //     closesocket(ConnectSocket);
       //     WSACleanup();
         //   return 1;
      //  }

      ///  if (!strcmp(sendbuf, "cao\n"))
      //      break;

      //  iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);


      //  std::cout<<  recvbuf ;

      //  ZeroMemory(sendbuf, sizeof(sendbuf));
     //   ZeroMemory(recvbuf, sizeof(recvbuf));


   // }




    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_BOTH);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }


    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}