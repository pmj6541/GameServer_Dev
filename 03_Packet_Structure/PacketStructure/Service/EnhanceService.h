#pragma once

#include "Domain/EnhanceResult.h"
#include "Domain/EnhanceData.h"

class EnhanceService
{
    public:
        static EnhanceResult Enhance(EnhanceData& data);
};
