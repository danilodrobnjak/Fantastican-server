#pragma once
#include <mutex>
#include <condition_variable>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>


std::mutex getOnlineClients_mutex;
std::condition_variable getOnlineClients_cv;
SOCKET getOnlineClients_value = INVALID_SOCKET;