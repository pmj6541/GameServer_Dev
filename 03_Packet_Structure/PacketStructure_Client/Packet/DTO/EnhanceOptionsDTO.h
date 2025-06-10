#pragma once

#include <string>
#include <sstream>
#include <stdexcept>
#include "../../Core/ByteBuffer.h"

struct EnhanceOptionsDTO {
    bool isStarcatch = false;
    bool isMemorial = false;

    void Serialize(ByteBuffer& buffer) const {
        buffer.Write(isStarcatch);
        buffer.Write(isMemorial);
    }

    void Deserialize(ByteBuffer& buffer) {
        buffer.Read(isStarcatch);
        buffer.Read(isMemorial);
    }
};