#pragma once

#include <unordered_map>
#include <memory>
#include "Handler.h"
#include "../Core/PacketOpcode.h"
#include <iostream>


class HandlerManager
{
    public:
        static HandlerManager& Instance()
        {
            static HandlerManager instance;
            return instance;
        }

        void RegisterHandler(EOpcode opcode, std::unique_ptr<Handler> handler)
        {
            handlers[opcode] = std::move(handler);
        }
        
        void Dispatch(EOpcode opcode, SOCKET clientSocket, const std::string& payload)
        {
            auto it = handlers.find(opcode);
            if (it != handlers.end())
            {
                it->second->Handle(clientSocket, payload);
            }
            else
            {
                std::cout << "Handler not found for opcode: " << static_cast<int>(opcode) << std::endl;
            }
        }
        


    private:
        HandlerManager() = default;
        ~HandlerManager() = default;
        
        std::unordered_map<EOpcode, std::unique_ptr<Handler>> handlers;
};
