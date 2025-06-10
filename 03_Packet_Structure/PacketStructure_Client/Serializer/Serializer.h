#pragma once
#include "../Core/ByteBuffer.h"
#include <memory>
#include <optional>

class Serializer {
public:
    static std::optional<uint16_t> GetOpCode(const uint8_t* data, size_t length) {
        if (length < sizeof(uint16_t)) return std::nullopt;

        uint16_t opCode;
        std::memcpy(&opCode, data, sizeof(uint16_t));
        return opCode;
    }

    // Serialize: OBJECT -> BYTE
    virtual void SerializeRequest(const void* object, ByteBuffer& buffer) const = 0;
    // Serialize: OBJECT -> BYTE
    virtual void SerializeResponse(const void* object, ByteBuffer& buffer) const = 0;

    // Deserialize: BYTE -> OBJECT
    virtual std::shared_ptr<void> DeserializeRequest(ByteBuffer& buffer) const = 0;
    // Deserialize: BYTE -> OBJECT
    virtual std::shared_ptr<void> DeserializeResponse(ByteBuffer& buffer) const = 0;


    virtual ~Serializer() = default;
};