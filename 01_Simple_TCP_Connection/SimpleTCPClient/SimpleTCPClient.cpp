#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib") // Winsock Library

using namespace std;

int main() {

    WSADATA wsaData;
    SOCKET clientSocket;
    SOCKADDR_IN serverAddr;
    
    // 원속 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed" << endl;
        return 1;
    }

    // 소켓 설정
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed" << endl;
        WSACleanup();
        return 1;
    }

    // 서버 주소 설정
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345); // 포트 번호 설정
    inet_pton(AF_INET, "127.0.0.1", &(serverAddr.sin_addr)); // 서버 IP 주소 설정

    // 서버에 연결
    if (connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Connection failed" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    cout << "Connected to server" << endl;

    // 서버에 메시지 전송
    const char* message = "Hello Server!";
    int sendResult = send(clientSocket, message, strlen(message), 0);
    if (sendResult == SOCKET_ERROR) {
        cerr << "Send failed." << endl;
    }
    else {
        cout << "Sent message to server!" << endl;
    }

    // 서버에게 받은 메시지
    char buffer[512];
    int recvSize = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (recvSize > 0) {
        buffer[recvSize] = '\0';
        cout << "Received from server: " << buffer << endl;
    }

    // 소켓 종료
    closesocket(clientSocket);
    WSACleanup();
    return 0;
    
    
}
