#pragma once

#include <unordered_map>
#include <memory>
#include "Handler.h"
#include "../Core/PacketOpcode.h"
#include <iostream>
#include "../Service/Domain/Domain.h"
#include "../Serializer/Serializer.h"


class HandlerManager
{
    public:
        static HandlerManager& Instance()
        {
            static HandlerManager instance;
            return instance;
        }

        void Register(EOpcode opcode, std::unique_ptr<IHandler> handler, std::unique_ptr<Serializer> serializer)
        {
            handlers[opcode] = std::move(handler);
            serializers[opcode] = std::move(serializer);
        }
        
        void Dispatch(EOpcode opcode, SOCKET clientSocket, ByteBuffer& buffer)
        {
            auto handler = handlers.find(opcode);
            auto serializer = serializers.find(opcode);
            if (handler != handlers.end())
            {
                std::shared_ptr<void> request = serializer->second->DeserializeRequest(buffer);
                handler->second->Handle(clientSocket, request);
            }
            else
            {
                std::cout << "Handler not found for opcode: " << static_cast<int>(opcode) << std::endl;
            }
        }
        


    private:
        HandlerManager() = default;
        ~HandlerManager() = default;
        
        std::unordered_map<EOpcode, std::unique_ptr<IHandler>> handlers;
        std::unordered_map<EOpcode, std::unique_ptr<Serializer>> serializers;
};
