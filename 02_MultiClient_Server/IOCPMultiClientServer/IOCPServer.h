#pragma once

#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>
#include <vector>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

enum class IOOperation
{
    Recv,
    Send
};

struct IOCPContext
{
    OVERLAPPED overlapped;
    WSABUF wsaBuf;
    char buffer[512];
    IOOperation operationType;

    IOCPContext(IOOperation type)
        : operationType(type)
    {
        ZeroMemory(&overlapped, sizeof(overlapped));
        wsaBuf.buf = buffer;
        wsaBuf.len = sizeof(buffer);
        ZeroMemory(buffer, sizeof(buffer));
    }
};

class IOCPServer {
    public:
        IOCPServer();
        ~IOCPServer();

        bool Initserver(int port, int workerThreadCount = 0);
        void Run();
        void Shutdown();

    private:
        void WorkerThread(); // 워커 스레드 함수
        bool RegisterClientSocket(SOCKET clientSocket); // 클라 소켓을 IOCP에 등록
        bool PostRecv(SOCKET clientSocket); // 비동기 Recv 등록

    private:
        SOCKET m_serverSocket = INVALID_SOCKET; // 리스닝 소켓
        HANDLE m_hIOCP = nullptr;               // IOCP 포트 핸들
        vector<thread> m_workerThreads;         // 워커 스레드 목록 
        bool m_bRunning = false;                // 서버 실행 상태
};
