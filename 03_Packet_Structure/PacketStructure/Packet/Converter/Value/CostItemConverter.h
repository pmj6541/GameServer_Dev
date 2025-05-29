#pragma once
#pragma once
#include "../../../Service/Domain/CostItem.h"
#include "../../DTO/CostItemDTO.h"

class CostItemConverter {
public:
    static CostItem ToDomain(const CostItemDTO& dto) {
        return CostItem{
            dto.ItemUID,
            dto.Quantity
        };
    }

    static CostItemDTO ToDTO(const CostItem& domain) {
        return CostItemDTO{
            domain.ItemUID,
            domain.Quantity
        };
    }
};
