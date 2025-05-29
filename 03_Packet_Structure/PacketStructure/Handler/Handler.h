#pragma once

#include <string>
#include <winsock2.h>

class Handler
{
    public:
        virtual ~Handler() = default;

        virtual void Handle(SOCKET clientSocket, const std::string& payload) = 0;
        
};