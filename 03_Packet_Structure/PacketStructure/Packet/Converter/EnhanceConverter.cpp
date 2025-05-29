#pragma once

#include "EnhanceConverter.h"
#include "../DTO/CostItemDTO.h"
#include "../../Service/Domain/CostItem.h"
#include "../DTO/ItemDTO.h"
#include "../../Service/Domain/Item.h"
#include "Value/CostItemConverter.h"
#include "Value/EnhanceOptionsConverter.h"
#include "Value/ItemConverter.h"

EnhanceData EnhanceConverter::ToDomain(const EnhanceRequest& request)
{
    std::vector<CostItem> costItems;
    costItems.reserve(request.CostItems.size());
    for (const auto& dto : request.CostItems) {
        costItems.push_back(CostItemConverter::ToDomain(dto));
    }

    return EnhanceData{
        request.UserId,
        request.ItemUID,
        request.EnhanceType,
        costItems,
        request.UseMeso,
        EnhanceOptionsConverter::ToDomain(request.Options)
    };
}

EnhanceResponse EnhanceConverter::ToDTO(const EnhanceResult& result)
{
    std::vector<long> usedItemIds = result.UsedItemIDs;

    return EnhanceResponse{
        ItemConverter::ToDTO(result.Item),
        usedItemIds,
        result.RestMeso,
        result.ResultCode
    };
}
