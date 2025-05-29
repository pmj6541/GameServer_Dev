#pragma once

#include <winsock2.h>
#include <windows.h>
#include <vector>
#include <thread>

class HandlerManager;

class IOCPServer {
public:
    IOCPServer();
    ~IOCPServer();

    bool Initialize(int port, int maxClients);
    void Run();
    void Stop();

private:
    bool CreateIOCP();
    bool CreateWorkerThreads();
    bool BindAndListen(int port);
    void AcceptConnections();
    void WorkerThread();

    HANDLE iocpHandle;
    SOCKET listenSocket;
    std::vector<std::thread> workerThreads;
    bool isRunning;
    HandlerManager& handlerManager;
};
