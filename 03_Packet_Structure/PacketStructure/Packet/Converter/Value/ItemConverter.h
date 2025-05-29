#pragma once
#include "../../../Service/Domain/Item.h"
#include "../../DTO/ItemDTO.h"

class ItemConverter {
public:
    static Item ToDomain(const ItemDTO& dto) {
        return Item{
            dto.ItemUID,
            dto.ItemID
        };
    }

    static ItemDTO ToDTO(const Item& domain) {
        return ItemDTO{
            domain.ItemUID,
            domain.ItemID
        };
    }
};
