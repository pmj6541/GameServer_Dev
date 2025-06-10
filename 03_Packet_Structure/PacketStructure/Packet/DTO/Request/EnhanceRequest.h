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


    std::string ToString() const {
        std::ostringstream oss;
        oss << "EnhanceRequest {\n";
        oss << "  UserId: " << UserId << "\n";
        oss << "  ItemUID: " << ItemUID << "\n";
        oss << "  EnhanceType: " << static_cast<int>(EnhanceType) << "\n";
        oss << "  UseMeso: " << UseMeso << "\n";
        oss << "  Options: { isStarcatch: " << Options.isStarcatch
            << ", isMemorial: " << Options.isMemorial << " }\n";
        oss << "  CostItems: [\n";
        for (const auto& item : CostItems) {
            oss << "    { ItemUID: " << item.ItemUID << ", Quantity: " << item.Quantity << " },\n";
        }
        oss << "  ]\n";
        oss << "}";
        return oss.str();
    }
};