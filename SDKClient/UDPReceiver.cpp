    #include "UDPReceiver.h"
    #include <iostream>
    #include <cstring>

    UDPReceiver::UDPReceiver(int port) : recvfd(-1) {
    #ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed." << std::endl;
            return;
        }
    #endif

        recvfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (recvfd < 0) {
            std::cerr << "UDPReceiver socket creation failed." << std::endl;
            return;
        }

        memset(&recvAddr, 0, sizeof(recvAddr));
        recvAddr.sin_family = AF_INET;
        recvAddr.sin_port = htons(port);
        recvAddr.sin_addr.s_addr = INADDR_ANY;

        if (bind(recvfd, (struct sockaddr*)&recvAddr, sizeof(recvAddr)) < 0) {
            std::cerr << "UDPReceiver bind failed." << std::endl;
            return;
        }

    #ifdef _WIN32
        u_long mode = 1;
        ioctlsocket(recvfd, FIONBIO, &mode);
    #else
        fcntl(recvfd, F_SETFL, O_NONBLOCK);
    #endif
    }

    UDPReceiver::~UDPReceiver() {
    #ifdef _WIN32
        closesocket(recvfd);
        WSACleanup();
    #else
        close(recvfd);
    #endif
    }
    
    char UDPReceiver::getCollisionCode() {
        char buffer[16] = { 0 };
        sockaddr_in senderAddr;
        int senderLen = sizeof(senderAddr);

        int n = recvfrom(recvfd, buffer, sizeof(buffer) - 1, 0,
                         (struct sockaddr*)&senderAddr, &senderLen);

        if (n == 1 && (buffer[0] == '1' || buffer[0] == '2')) {
            return buffer[0];  // '1' = left, '2' = right
        }

        return 0;  // no new valid message
    }