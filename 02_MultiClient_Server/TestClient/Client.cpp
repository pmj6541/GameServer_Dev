#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

atomic<int> successCount(0);

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
        string message = "Message " + to_string(i) + " from client " + to_string(id) + "\n";
        send(sock, message.c_str(), message.size(), 0);
        // 필요하면 Sleep 추가
        // Sleep(10);
    }

    char buffer[512];
    string pendingData;
    int totalRecvCount = 0;

    while (totalRecvCount < MESSAGE_COUNT)
    {
        int recvSize = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (recvSize > 0)
        {
            buffer[recvSize] = '\0';
            pendingData += buffer;

            size_t pos;
            while ((pos = pendingData.find('\n')) != string::npos)
            {
                string completeMessage = pendingData.substr(0, pos);
                // cout << "Received: " << completeMessage << endl;
                pendingData.erase(0, pos + 1);
                totalRecvCount++;
            }
        }
        else if (recvSize == 0)
        {
           // cout << "Server closed connection." << endl;
            break;
        }
        else
        {
            // cout << "Recv failed." << endl;
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

    cout << "Input MESSAGE_COUNT (each client): ";
    cin >> MESSAGE_COUNT;

    vector<thread> clientThreads;

    auto startTime = chrono::high_resolution_clock::now();

    for (int i = 0; i < CLIENT_COUNT; ++i)
    {
        clientThreads.emplace_back(ConnectAndSend, i);
    }

    for (auto& t : clientThreads)
    {
        if (t.joinable())
            t.join();
    }

    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = endTime - startTime;

    cout << "Total elapsed time: " << elapsed.count() << " seconds" << endl;
    cout << "Success count: " << successCount << " / " << CLIENT_COUNT << endl;
    cout << "Success rate: " << (double(successCount) / CLIENT_COUNT) * 100.0 << "%" << endl;

    WSACleanup();
    return 0;
}
