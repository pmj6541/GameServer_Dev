#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib") // Winsock Library

using namespace std;

int main() {

    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    SOCKADDR_IN serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    // 원속 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed" << endl;
        return 1;
    }

    // 서버 소켓 생성
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed" << endl;
        WSACleanup();
        return 1;
    }

    // 서버 주소 설정
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345); // 포트 번호 설정
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 네트워크 인터페이스에서 수신

    // 소켓 바인딩
    if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Bind failed" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // 리스닝 시작
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "Listen failed" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    cout << "Server is Listening..." << endl;

    // 클라이언트 접속 대기
    clientAddrSize = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Accept failed" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    cout << "Client connected!" << endl;

    // 클라이언트와 메시지 주고받기
    char buffer[512];
    int recvSize = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (recvSize > 0) {
        buffer[recvSize] = '\0';
        cout << "Received from client: " << buffer << endl;

        string response = "Hello from server!";
        send(clientSocket, response.c_str(), response.length(), 0);
    }

    // 소켓 종료
    closesocket(clientSocket);
    closesocket(serverSocket);

    // Winsock 종료
    WSACleanup();
    return 0;
    
}
