#include "UDPSender.h"
#include <iostream>
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <unistd.h>
#include <arpa/inet.h>
#endif

UDPSender::UDPSender(const std::string& ip, int port) {
	sockfd = -1;
	recvfd = -1;

	// Initialize Winsock (Windows only)


#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return;
    }
#endif

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Socket creation failed." << std::endl;
        return;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

    //Receiver setup 
	recvfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (recvfd < 0) {
		std::cerr << "Receiver socket creation failed." << std::endl;
		return;
	}

	
	memset(&recvAddr, 0, sizeof(recvAddr));
	this->recvAddr.sin_family = AF_INET;
	this->recvAddr.sin_port = htons(6006);
	this->recvAddr.sin_addr.s_addr = INADDR_ANY;

	#ifdef _WIN32
	u_long mode = 1; // Non-blocking mode
	ioctlsocket(recvfd, FIONBIO, &mode);
    #else
	fcntl(recvfd, F_SETFL, O_NONBLOCK);
    #endif

}

UDPSender::~UDPSender() {
#ifdef _WIN32
    closesocket(sockfd);
	closesocket(recvfd);
    WSACleanup();
#else
    close(sockfd);
	close(recvfd);
#endif
}

bool UDPSender::sendData(const float sendBuffer[40]) {


    size_t bytesToSend = sizeof(float) * 40;

    return sendto(sockfd, reinterpret_cast<const char*>(sendBuffer), bytesToSend, 0,
                  (struct sockaddr*)&serverAddr, sizeof(serverAddr)) >= 0;

}


bool UDPSender::receiveToggle(bool& outToggle) {
    char buffer[16] = { 0 };
    sockaddr_in senderAddr;
    int senderLen = sizeof(senderAddr);
    int n = recvfrom(recvfd, buffer, sizeof(buffer) - 1, 0,
        (struct sockaddr*)&senderAddr, &senderLen);

    if (n > 0) {
        buffer[n] = '\0'; // Null-terminate
        if (strcmp(buffer, "1") == 0) {
            outToggle = true;
            return true;
        }
        else if (strcmp(buffer, "0") == 0) {
            outToggle = false;
            return true;
        }
    }

    return false; // No toggle received
}



