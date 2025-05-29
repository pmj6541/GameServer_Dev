#pragma once

#include "Serializer.h"
#include "../Packet/DTO/Request/EnhanceRequest.h"
#include "../Packet/DTO/Response/EnhanceResponse.h"
#include <stdexcept>

class EnhanceSerializer : public Serializer {
public:
    void Serialize(const void* object, ByteBuffer& buffer) const override {
        const EnhanceResponse* response = static_cast<const EnhanceResponse*>(object);
        if (!response) {
            throw std::runtime_error("EnhanceSerializer::Serialize - Invalid object cast");
        }

        response->Item.Serialize(buffer);

        uint16_t usedCount = static_cast<uint16_t>(response->UsedItemIDs.size());
        buffer.Write(usedCount);
        for (auto id : response->UsedItemIDs) {
            buffer.Write(id);
        }

        buffer.Write(response->RestMeso);

        int32_t resultCode = static_cast<int32_t>(response->ResultCode);
        buffer.Write(resultCode);
    }

    std::shared_ptr<void> Deserialize(ByteBuffer& buffer) const override {
        auto request = std::make_shared<EnhanceRequest>();

        buffer.Read(request->UserId);
        buffer.Read(request->ItemUID);

        int32_t enhanceTypeRaw;
        buffer.Read(enhanceTypeRaw);
        request->EnhanceType = static_cast<EnhanceType>(enhanceTypeRaw);

        uint16_t costItemCount;
        buffer.Read(costItemCount);
        request->CostItems.resize(costItemCount);
        for (auto& costItem : request->CostItems) {
            costItem.Deserialize(buffer);
        }

        buffer.Read(request->UseMeso);
        request->Options.Deserialize(buffer);

        return request;
    }
};