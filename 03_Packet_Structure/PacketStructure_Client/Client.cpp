#include <winsock2.h>
#include <iostream>
#include <Ws2tcpip.h>
#include "./Core/ByteBuffer.h"
#include "./Packet/DTO/Request/EnhanceRequest.h"
#include "./Serializer/EnhanceSerializer.h"
#include "./Core/PacketOpcode.h"

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;

    // 1. WinSock 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }

    // 2. 소켓 생성
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return 1;
    }

    // 3. 서버 주소 설정
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9000);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    // 4. 서버에 연결
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed.\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server.\n";

    // 5. 요청 DTO 준비
    EnhanceRequest req;
    req.UserId = 1;
    req.ItemUID = 12345678;
    req.EnhanceType = EnhanceType::None;
    req.UseMeso = true;
    // req.CostItems, req.Options 등은 생략되었으니 기본값 또는 직접 설정 필요

    // 6. 직렬화
    std::cout << "Serialize Start\n";
    EnhanceSerializer serializer;
    ByteBuffer buffer;

    // 먼저 OpCode 기록 (EnhanceRequest: 101)
    buffer.Write(static_cast<uint16_t>(EOpcode::EnhanceRequest));

    serializer.SerializeRequest(&req, buffer);



    // 7. 전송
    std::cout << "Send Start\n";
    const std::vector<uint8_t>& bytes = buffer.GetData();
    int sendResult = send(clientSocket, reinterpret_cast<const char*>(bytes.data()), static_cast<int>(bytes.size()), 0);

    if (sendResult == SOCKET_ERROR) {
        std::cerr << "Send failed.\n";
    }
    else {
        std::cout << "EnhanceRequest sent (" << sendResult << " bytes)\n";
    }



    // 8. 종료
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
