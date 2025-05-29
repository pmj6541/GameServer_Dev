#pragma once
#include "../../../Service/Domain/EnhanceOptions.h"
#include "../../DTO/EnhanceOptionsDTO.h"
#include "../../DTO/EnhanceOptionsDTO.h"

class EnhanceOptionsConverter {
public:
    static EnhanceOptions ToDomain(const EnhanceOptionsDTO& dto) {
        return EnhanceOptions{
            dto.isStarcatch,
            dto.isMemorial
        };
    }

    static EnhanceOptionsDTO ToDTO(const EnhanceOptions& domain) {
        return EnhanceOptionsDTO{
            domain.isStarcatch,
            domain.isMemorial
        };
    }
};
