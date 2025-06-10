#pragma once
#include <vector>
#include "../../Core/EnhanceType.h"
#include "EnhanceOptions.h"
#include "CostItem.h"
#include "Domain.h"

struct EnhanceData : public Domain
{
    long UserId;
    long ItemUID;
    EnhanceType EnhanceType;
    std::vector<CostItem> CostItems;
    long UseMeso;
    EnhanceOptions Options;
};

