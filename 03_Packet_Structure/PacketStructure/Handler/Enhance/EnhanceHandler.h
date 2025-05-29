#pragma once

#include <string>
#include <winsock2.h>
#include <iostream>
#include "../Handler.h"
#include "../../Packet/DTO/Request/EnhanceRequest.h"
#include "../../Packet/DTO/Response/EnhanceResponse.h"
#include "../../Serializer/EnhanceSerializer.h"
#include "../../Service/Domain/EnhanceData.h"
#include "../../Service/Domain/EnhanceResult.h"



class EnhanceHandler : public Handler
{
    public:
        void Handle(SOCKET clientSocket, const std::string& payload) override;
};
