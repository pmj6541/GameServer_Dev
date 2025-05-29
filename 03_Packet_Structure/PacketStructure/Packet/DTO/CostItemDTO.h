#pragma once

#include <string>
#include <sstream>
#include <stdexcept>
#include "../../Core/ByteBuffer.h"

struct CostItemDTO {
    long ItemUID;
    int Quantity;

    void Serialize(ByteBuffer& buffer) const {
        buffer.Write(ItemUID);
        buffer.Write(Quantity);
    }

    void Deserialize(ByteBuffer& buffer) {
        buffer.Read(ItemUID);
        buffer.Read(Quantity);
    }
};