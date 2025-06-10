#pragma once
#include "../EnhanceOptionsDTO.h"
#include "../../../Core/EnhanceType.h"
#include "../CostItemDTO.h"

struct EnhanceRequest
{
    long UserId;
    long ItemUID;
    EnhanceType EnhanceType;
    std::vector<CostItemDTO> CostItems;
    long UseMeso;
    EnhanceOptionsDTO Options;
};
