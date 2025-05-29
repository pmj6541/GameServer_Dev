#pragma once

#include "../DTO/Request/EnhanceRequest.h"
#include "../DTO/Response/EnhanceResponse.h"
#include "../../Service/Domain/EnhanceData.h"
#include "../../Service/Domain/EnhanceResult.h"

class EnhanceConverter
{
    public:
        static EnhanceData ToDomain(const EnhanceRequest& request);
        static EnhanceResponse ToDTO(const EnhanceResult& result);
};
