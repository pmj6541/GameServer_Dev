#pragma once
#include <vector>
#include "Item.h"
#include "../../Core/ResultCode.h"

struct EnhanceResult
{
    Item Item;
    std::vector<long> UsedItemIDs;
    long RestMeso;
    ResultCode ResultCode;
};