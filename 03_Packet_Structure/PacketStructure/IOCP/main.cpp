#include "IOCPServer.h"
#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")  // 윈속 라이브러리 링크

int main() {
    // Winsock 초기화
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    IOCPServer server;

    const int port = 9000;
    const int maxClients = 100;

    if (!server.Initialize(port, maxClients)) {
        std::cerr << "Server initialization failed." << std::endl;
        WSACleanup();
        return 1;
    }

    server.Run();  // Accept loop 시작

    // 종료 시
    WSACleanup();
    return 0;
}
