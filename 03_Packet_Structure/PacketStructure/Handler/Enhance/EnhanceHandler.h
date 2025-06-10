#pragma once

#include <string>
#include <winsock2.h>
#include <iostream>
#include "../Handler.h"
#include "../../Packet/DTO/Request/EnhanceRequest.h"

class EnhanceHandler : public Handler<EnhanceRequest>
{
    public:
        void HandleImpl(SOCKET clientSocket, const EnhanceRequest& data) override;
};
