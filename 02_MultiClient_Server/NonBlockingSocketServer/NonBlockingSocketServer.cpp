#include <iostream>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h> // inet_pton 함수 사용
#pragma comment(lib, "ws2_32.lib")

#define BLOCKING_MODE 0
#define NON_BLOCKING_MODE 1

using namespace std;

SOCKET serverSocket = INVALID_SOCKET;
vector<SOCKET> clientSockets;
bool g_bRunning = true;

bool InitServer(){
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup failed" << endl;
        return false;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(serverSocket == INVALID_SOCKET){
        cerr << "Socket creation failed." << endl;
        WSACleanup();
        return false;
    }

    // Non-Blocking 모드 설정
    u_long mode = NON_BLOCKING_MODE;
    if (ioctlsocket(serverSocket, FIONBIO, &mode) == SOCKET_ERROR) {
        cerr << "Failed to set non-blocking mode." << endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }

    // 서버 주소 설정
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(12345);
    
    // 서버 소켓 바인딩
    if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Socket binding failed." << endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "Listen failed." << endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }

    cout << "Server is running on port 12345..." << endl;
    return true;
}

void ShutdownServer() {
    cout << "Shutting down server..." << endl;
    for (SOCKET client : clientSockets) {
        closesocket(client);
    }
    closesocket(serverSocket);
    WSACleanup();
}

int main() {
    if (!InitServer()) {
        return 1;
    }

    fd_set readSet;
    timeval timeout;

    while (g_bRunning) {
        FD_ZERO(&readSet);

        // 서버 소켓 등록
        FD_SET(serverSocket, &readSet);

        // 클라이언트 소켓들 등록
        for (SOCKET client : clientSockets) {
            FD_SET(client, &readSet);
        }

        // 타임아웃 설정 (1초 대기)
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int selectResult = select(0, &readSet, nullptr, nullptr, &timeout);
        cout << "select is called." << endl;
        if (selectResult == SOCKET_ERROR) {
            cerr << "select() failed." << endl;
            break;
        }

        if (selectResult == 0) {
            // 타임아웃 발생 (1초 동안 이벤트 없음)
            continue;
        }

        // 서버 소켓에 이벤트가 있으면 새로운 클라이언트 accept
        if (FD_ISSET(serverSocket, &readSet)) {
            SOCKADDR_IN clientAddr = {};
            int addrLen = sizeof(clientAddr);

            SOCKET clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &addrLen);
            if (clientSocket != INVALID_SOCKET) {
                cout << "New client connected!" << endl;

                // Non-Blocking 모드 설정
                u_long mode = 1;
                ioctlsocket(clientSocket, FIONBIO, &mode);

                clientSockets.emplace_back(clientSocket);
            }
        }

        // 클라이언트 소켓에 이벤트가 있으면 데이터 recv
        vector<SOCKET> disconnectedClients;
        for (SOCKET client : clientSockets) {
            if (FD_ISSET(client, &readSet)) {
                char buffer[512];
                int recvSize = recv(client, buffer, sizeof(buffer), 0);

                if (recvSize <= 0) {
                    cout << "client disconnected!" << endl;
                    closesocket(client);
                    disconnectedClients.emplace_back(client);
                } else {
                    buffer[recvSize] = '\0';
                    cout << "Received message: " << buffer << endl;

                    // 에코
                    send(client, buffer, recvSize, 0);
                }
            }
        }

        // 종료된 클라이언트 제거
        for (SOCKET disconnectedClient : disconnectedClients) {
            clientSockets.erase(remove(clientSockets.begin(), clientSockets.end(), disconnectedClient), clientSockets.end());
        }
    }

    ShutdownServer();
    cout << "Server shutdown complete." << endl;
    return 0;
}