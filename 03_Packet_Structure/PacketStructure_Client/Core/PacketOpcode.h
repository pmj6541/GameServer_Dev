#pragma once

#include <cstdint>

enum class EOpcode : uint16_t
{
    None = 0,

    Enhance = 100,
    EnhanceRequest = 101,
    EnhanceResponse = 102,
};
