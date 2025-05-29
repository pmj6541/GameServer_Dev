#include "IOCPServer.h"
#include <iostream>
#include "../Core/RecvContext.h"
#include "../Handler/Handler.h"
#include "../Handler/HandlerManager.h"
#include "../Core/PacketOpcode.h"
#include "../Handler/Enhance/EnhanceHandler.h"
#include "../Serializer/Serializer.h"
#include "../Serializer/EnhanceSerializer.h"

IOCPServer::IOCPServer()
    : iocpHandle(nullptr), listenSocket(INVALID_SOCKET), isRunning(false), handlerManager(HandlerManager::Instance()) {}

IOCPServer::~IOCPServer() {
    Stop();
}

bool IOCPServer::Initialize(int port, int maxClients) {
    if (!CreateIOCP()) {
        std::cerr << "Failed to create IOCP." << std::endl;
        return false;
    }

    if (!BindAndListen(port)) {
        std::cerr << "Failed to bind and listen on port " << port << "." << std::endl;
        return false;
    }

    if (!CreateWorkerThreads()) {
        std::cerr << "Failed to create worker threads." << std::endl;
        return false;
    }

    return true;
}

void IOCPServer::Run() {
    handlerManager
        .RegisterHandler(EOpcode::EnhanceRequest, std::make_unique<EnhanceHandler>());

    isRunning = true;
    std::cout << "Server is running..." << std::endl;

    AcceptConnections();
}

void IOCPServer::Stop() {
    isRunning = false;

    // Close the listen socket
    if (listenSocket != INVALID_SOCKET) {
        closesocket(listenSocket);
        listenSocket = INVALID_SOCKET;
    }

    // Close IOCP handle
    if (iocpHandle) {
        CloseHandle(iocpHandle);
        iocpHandle = nullptr;
    }

    // Join all worker threads
    for (auto& thread : workerThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    workerThreads.clear();
    std::cout << "Server stopped." << std::endl;
}

bool IOCPServer::CreateIOCP() {
    iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
    return iocpHandle != nullptr;
}

bool IOCPServer::CreateWorkerThreads() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    int numThreads = sysInfo.dwNumberOfProcessors * 2;

    try {
        for (int i = 0; i < numThreads; ++i) {
            workerThreads.emplace_back(&IOCPServer::WorkerThread, this);
        }
    } catch (...) {
        return false;
    }

    return true;
}

bool IOCPServer::BindAndListen(int port) {
    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == INVALID_SOCKET) {
        return false;
    }

    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(listenSocket);
        return false;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        closesocket(listenSocket);
        return false;
    }

    return true;
}

void IOCPServer::AcceptConnections() {
    while (isRunning) {
        SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            if (isRunning) {
                std::cerr << "Failed to accept connection." << std::endl;
            }
            continue;
        }

        // Associate the client socket with the IOCP
        if (CreateIoCompletionPort((HANDLE)clientSocket, iocpHandle, (ULONG_PTR)clientSocket, 0) == nullptr) {
            closesocket(clientSocket);
            std::cerr << "Failed to associate client socket with IOCP." << std::endl;
        } else {
            std::cout << "Client connected." << std::endl;
        }
    }
}

void IOCPServer::WorkerThread() {
    //while (isRunning) {
    //    DWORD bytesTransferred = 0;
    //    ULONG_PTR completionKey = 0;
    //    OVERLAPPED* overlapped = nullptr;

    //    BOOL result = GetQueuedCompletionStatus(
    //        iocpHandle, &bytesTransferred, &completionKey, &overlapped, INFINITE);

    //    if (!result || bytesTransferred == 0) {
    //        if (completionKey) {
    //            SOCKET clientSocket = (SOCKET)completionKey;
    //            closesocket(clientSocket);
    //            std::cout << "Client disconnected." << std::endl;
    //        }
    //        continue;
    //    }

    //    
    //    RecvContext* context = reinterpret_cast<RecvContext*>(overlapped);
    //    const uint8_t* rawData = context->buffer;

    //    if (bytesTransferred > 0) {

    //        
    //        auto opcodeOpt = Serializer::GetOpCode(rawData, bytesTransferred);
    //        if (!opcodeOpt.has_value()) {
    //            std::cerr << "Invalid opcode in received message." << std::endl;
    //            continue;
    //        }

    //        ByteBuffer buffer(rawData, static_cast<size_t>(bytesTransferred));

    //        switch (static_cast<EOpcode>(opcodeOpt.value())) {
    //        case EOpcode::EnhanceRequest: {
    //            EnhanceSerializer serializer;
    //            EnhanceRequest request;
    //            if (serializer.Deserialize(buffer)) {
    //                std::cout << "EnhanceRequest deserialized successfully." << std::endl;
    //                // ADD LOGIC
    //            }
    //            break;
    //        }
    //        default:
    //            std::cerr << "Unhandled opcode: " << static_cast<int>(opcodeOpt.value()) << std::endl;
    //            break;
    //        }
    //    }
    //}
}
