#pragma once

#include <string>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <unistd.h>
#include <arpa/inet.h>  
#endif

class UDPReceiver {
public:
    UDPReceiver(int port);
    ~UDPReceiver();

    char getCollisionCode();


private:
    int recvfd;
    sockaddr_in recvAddr;
};