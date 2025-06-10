#pragma once

#include <string>
#include <sstream>
#include <stdexcept>
#include "../../Core/ByteBuffer.h"

struct ItemDTO {
    long ItemUID;
    long ItemID;

    void Serialize(ByteBuffer& buffer) const {
        buffer.Write(ItemUID);
        buffer.Write(ItemID);
    }

    void Deserialize(ByteBuffer& buffer) {
        buffer.Read(ItemUID);
        buffer.Read(ItemID);
    }
};
