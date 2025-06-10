#include "IOCPServer.h"
#include <iostream>

IOCPServer::IOCPServer() {

}

IOCPServer::~IOCPServer() {
    Shutdown();
}

bool IOCPServer::Initserver(int port, int workerThreadCount) {
    // 윈속 초기화
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "WSAStartup failed." << std::endl;
        return false;
    }

    m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_serverSocket == INVALID_SOCKET) {
        std::cout << "socket failed." << std::endl;
        WSACleanup();
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(m_serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "bind failed." << std::endl;
        closesocket(m_serverSocket);
        WSACleanup();
        return false;
    }

    if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "listen failed." << std::endl;
        closesocket(m_serverSocket);
        WSACleanup();
        return false;
    }

    m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (m_hIOCP == NULL) {
        std::cerr << "CreateIoCompletionPort failed." << std::endl;
        closesocket(m_serverSocket);
        WSACleanup();
        return false;
    }

    m_bRunning = true;
    
    // 워커 스레드 개수 결정 (기본 cpu * 2)
    if (workerThreadCount == 0) {
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        workerThreadCount = sysInfo.dwNumberOfProcessors * 2;
    }

    for (int i = 0; i < workerThreadCount; i++) {
        m_workerThreads.emplace_back(&IOCPServer::WorkerThread, this);
    }
    std::cout << "Server initialized and listening on port " << port << std::endl;

    return true;
}

void IOCPServer::Run() {
    while (m_bRunning) {
        sockaddr_in clientAddr;
        int addrLen = sizeof(clientAddr);

        SOCKET clientSocket = accept(m_serverSocket, (SOCKADDR*)&clientAddr, &addrLen);
        if (clientSocket == INVALID_SOCKET) {
            if (!m_bRunning) break;
            // cerr << "accept failed." << endl;
            continue;
        }
        // cout << "Client connected: " << clientSocket << endl;

        RegisterClientSocket(clientSocket);
        PostRecv(clientSocket);
    }
}

void IOCPServer::Shutdown() {
    if (!m_bRunning) return;

    m_bRunning = false;

    if (m_serverSocket != INVALID_SOCKET) {
        closesocket(m_serverSocket);
        m_serverSocket = INVALID_SOCKET;
    }

    if (m_hIOCP) {
        for (size_t i = 0; i < m_workerThreads.size(); i++) {
            PostQueuedCompletionStatus(m_hIOCP, 0, 0, NULL);
        }
    }

    for (auto& thread : m_workerThreads) {
        if (thread.joinable())
            thread.join();
    }

    if (m_hIOCP) {
        CloseHandle(m_hIOCP);
        m_hIOCP = NULL;
    }

    WSACleanup();
    std::cout << "Server shutdown complete." << std::endl;
        
}
void IOCPServer::WorkerThread()
{
    while (m_bRunning)
    {
        DWORD transferred = 0;
        ULONG_PTR key = 0;
        LPOVERLAPPED overlapped = nullptr;

        BOOL result = GetQueuedCompletionStatus(
            m_hIOCP,
            &transferred,
            &key,
            &overlapped,
            INFINITE
        );

        if (!result || overlapped == nullptr)
        {
            break; // 종료
        }

        SOCKET clientSocket = (SOCKET)key;
        IOCPContext* context = reinterpret_cast<IOCPContext*>(overlapped);

        if (context->operationType == IOOperation::Recv)
        {
            if (transferred == 0)
            {
                // 클라이언트 연결 끊김
                closesocket(clientSocket);
                delete context;
                continue;
            }

            // 받은 데이터 출력
            context->buffer[transferred] = '\0'; // 널 종료
            // cout << "Received: " << context->buffer << endl;

            // Send용 context 새로 생성
            IOCPContext* sendContext = new IOCPContext(IOOperation::Send);
            memcpy(sendContext->buffer, context->buffer, transferred);
            sendContext->wsaBuf.len = transferred;

            DWORD sendBytes = 0;
            int sendResult = WSASend(clientSocket, &sendContext->wsaBuf, 1, &sendBytes, 0, &sendContext->overlapped, NULL);
            if (sendResult == SOCKET_ERROR)
            {
                int err = WSAGetLastError();
                if (err != WSA_IO_PENDING)
                {
                    // cerr << "WSASend failed: " << err << endl;
                    closesocket(clientSocket);
                    delete sendContext;
                }
            }

            // Recv context는 여기서 정리
            delete context;

            // 다시 다음 수신 등록
            PostRecv(clientSocket);
        }
        else if (context->operationType == IOOperation::Send)
        {
            // Send 완료 처리
            delete context; // Send용 context는 여기서 삭제
        }
    }
}


bool IOCPServer::RegisterClientSocket(SOCKET clientSocket) {
    if (CreateIoCompletionPort((HANDLE)clientSocket, m_hIOCP, (ULONG_PTR)clientSocket, 0 ) == NULL) {
        // cerr << "Failed to associate client socket with IOCP." << endl;
        closesocket(clientSocket);
        return false;
    }
    return true;
}

bool IOCPServer::PostRecv(SOCKET clientSocket)
{
    IOCPContext* context = new IOCPContext(IOOperation::Recv);

    DWORD flags = 0;
    DWORD recvBytes = 0;

    int ret = WSARecv(clientSocket, &context->wsaBuf, 1, &recvBytes, &flags, &context->overlapped, NULL);
    if (ret == SOCKET_ERROR)
    {
        int err = WSAGetLastError();
        if (err != WSA_IO_PENDING)
        {
            // cerr << "WSARecv failed with error: " << err << endl;
            closesocket(clientSocket);
            delete context;
            return false;
        }
    }
    return true;
}
