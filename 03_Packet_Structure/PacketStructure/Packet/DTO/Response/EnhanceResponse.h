#pragma once
#include <vector>
#include "../ItemDTO.h"
#include "../../Core/ResultCode.h"

struct EnhanceResponse
{
    ItemDTO Item;
    std::vector<long> UsedItemIDs;
    long RestMeso;
    ResultCode ResultCode;
};