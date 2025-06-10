#pragma once

#include "EnhanceHandler.h"
#include "../../Packet/Converter/EnhanceConverter.h"
#include "../../Serializer/EnhanceSerializer.h"
#include "../../Packet/DTO/Request/EnhanceRequest.h"

void EnhanceHandler::HandleImpl(SOCKET clientSocket, const EnhanceRequest& payload)
{
    std::cout << "Im EnhanceHandler's Handle" << std::endl;
    std::cout << "I got this." << std::endl;
    std::cout << payload.ToString() << std::endl;
}
