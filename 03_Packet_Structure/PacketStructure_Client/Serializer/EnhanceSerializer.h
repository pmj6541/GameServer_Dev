#pragma once

#include "Serializer.h"
#include "../Packet/DTO/Request/EnhanceRequest.h"
#include "../Packet/DTO/Response/EnhanceResponse.h"
#include <stdexcept>

class EnhanceSerializer : public Serializer {
public:
    void SerializeRequest(const void* object, ByteBuffer& buffer) const {
        const EnhanceRequest* request = static_cast<const EnhanceRequest*>(object);
        if (!request) {
            throw std::runtime_error("EnhanceSerializer::SerializeRequest - Invalid object cast");
        }

        buffer.Write(request->UserId);
        buffer.Write(request->ItemUID);

        int32_t enhanceTypeRaw = static_cast<int32_t>(request->EnhanceType);
        buffer.Write(enhanceTypeRaw);

        uint16_t costCount = static_cast<uint16_t>(request->CostItems.size());
        buffer.Write(costCount);
        for (const auto& costItem : request->CostItems) {
            costItem.Serialize(buffer);
        }

        buffer.Write(request->UseMeso);
        request->Options.Serialize(buffer);
    }

    void SerializeResponse(const void* object, ByteBuffer& buffer) const override {
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

    std::shared_ptr<void> DeserializeRequest(ByteBuffer& buffer) const override {
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
    std::shared_ptr<void> DeserializeResponse(ByteBuffer& buffer) const override {
        auto response = std::make_shared<EnhanceResponse>();

        response->Item.Deserialize(buffer);

        uint16_t usedItemCount;
        buffer.Read(usedItemCount);
        response->UsedItemIDs.resize(usedItemCount);
        for (auto& id : response->UsedItemIDs) {
            buffer.Read(id);
        }

        buffer.Read(response->RestMeso);

        int32_t resultCodeRaw;
        buffer.Read(resultCodeRaw);
        response->ResultCode = static_cast<ResultCode>(resultCodeRaw);

        return response;
    }

};