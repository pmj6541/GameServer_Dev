#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

atomic<bool> g_bRunning(true); // 서버 실행 플래그
SOCKET serverSocket = INVALID_SOCKET;

void HandleClient(SOCKET clientSocket)
{
    char buffer[512];
    while (true)
    {
        int recvSize = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (recvSize <= 0) {
            cout << "Client disconnected." << endl;
            break;
        }

        buffer[recvSize] = '\0';
        cout << "Received from client: " << buffer << endl;

        send(clientSocket, buffer, recvSize, 0); // 에코
    }

    closesocket(clientSocket);
}

BOOL WINAPI ConsoleHandler(DWORD signal)
{
    if (signal == CTRL_C_EVENT)
    {
        cout << "\nCtrl+C detected. Shutting down server..." << endl;
        g_bRunning = false;

        if (serverSocket != INVALID_SOCKET)
        {
            closesocket(serverSocket);  // 서버 소켓을 닫아버린다
        }
    }
    return TRUE;
}


int main()
{
    WSADATA wsaData;
    
    SOCKADDR_IN serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    vector<thread> clientThreads;

    // 시그널 핸들러 등록
    SetConsoleCtrlHandler(ConsoleHandler, TRUE);

    // 윈속 초기화
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // 서버 소켓 생성
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // 서버 주소 설정
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // 바인딩
    bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));

    // 리스닝 시작
    listen(serverSocket, SOMAXCONN);
    cout << "Server listening..." << endl;

    // 메인 서버 루프
    while (g_bRunning)
    {
        SOCKET clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &clientAddrSize);
        if (clientSocket == INVALID_SOCKET) {
            if (!g_bRunning) break; // 서버 종료 신호 받으면 루프 탈출
            cerr << "Accept failed." << endl;
            continue;
        }

        cout << "Client connected!" << endl;
        clientThreads.emplace_back(HandleClient, clientSocket);
    }

    // 모든 스레드 정리
    for (auto& t : clientThreads) {
        if (t.joinable())
            t.join();
    }

    // 윈속 종료
    WSACleanup();

    cout << "Server shutdown complete." << endl;
    return 0;
}
