#pragma once
#include <vector>
#include "../../Core/EnhanceType.h"
#include "EnhanceOptions.h"
#include "CostItem.h"

struct EnhanceData
{
    long UserId;
    long ItemUID;
    EnhanceType EnhanceType;
    std::vector<CostItem> CostItems;
    long UseMeso;
    EnhanceOptions Options;
};

