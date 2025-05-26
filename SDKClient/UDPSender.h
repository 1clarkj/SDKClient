#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <string>
#include "ManusSDK.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

class UDPSender {
public:
    UDPSender(const std::string& ip, int port);
    ~UDPSender();

    bool sendData(const float sendBuffer[40]);

    bool receiveToggle(bool& toggle);


private:
    int sockfd;
    struct sockaddr_in serverAddr;


    //Receiver 
    int recvfd;
	struct sockaddr_in recvAddr;
    static constexpr int RECV_PORT = 6006;
};

#endif
