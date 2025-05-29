#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>

#pragma comment(lib, "ws2_32.lib")

std::atomic<int> successCount(0);

int MESSAGE_COUNT = 10; // 연결당 보낼 메시지 수 (전역 변수)

void ConnectAndSend(int id)
{
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        return;
    }

    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        closesocket(sock);
        return;
    }

    for (int i = 0; i < MESSAGE_COUNT; i++) {
        std::string message = "Message " + std::to_string(i) + " from client " + std::to_string(id) + "\n";
        send(sock, message.c_str(), message.size(), 0);
        // 필요하면 Sleep 추가
        // Sleep(10);
    }

    char buffer[512];
    std::string pendingData;
    int totalRecvCount = 0;

    while (totalRecvCount < MESSAGE_COUNT)
    {
        int recvSize = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (recvSize > 0)
        {
            buffer[recvSize] = '\0';
            pendingData += buffer;

            size_t pos;
            while ((pos = pendingData.find('\n')) != std::string::npos)
            {
                std::string completeMessage = pendingData.substr(0, pos);
                // std::cout << "Received: " << completeMessage << std::endl;
                pendingData.erase(0, pos + 1);
                totalRecvCount++;
            }
        }
        else if (recvSize == 0)
        {
           // std::cout << "Server closed connection." << std::endl;
            break;
        }
        else
        {
            // std::cout << "Recv failed." << std::endl;
            break;
        }
    }

    successCount++;
    closesocket(sock);
}

int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    const int CLIENT_COUNT = 1000; // 테스트할 클라이언트 수

    std::cout << "Input MESSAGE_COUNT (each client): ";
    std::cin >> MESSAGE_COUNT;

    std::vector<std::thread> clientThreads;

    auto startTime = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < CLIENT_COUNT; ++i)
    {
        clientThreads.emplace_back(ConnectAndSend, i);
    }

    for (auto& t : clientThreads)
    {
        if (t.joinable())
            t.join();
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;

    std::cout << "Total elapsed time: " << elapsed.count() << " seconds" << std::endl;
    std::cout << "Success count: " << successCount << " / " << CLIENT_COUNT << std::endl;
    std::cout << "Success rate: " << (double(successCount) / CLIENT_COUNT) * 100.0 << "%" << std::endl;

    WSACleanup();
    return 0;
}
