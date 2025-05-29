#pragma once
#include <vector>
#include <string>
#include <cstring>
#include <stdexcept>
#include <cstdint>

class ByteBuffer {
private:
    std::vector<uint8_t> Buffer;
    size_t ReadPos = 0;

public:
    ByteBuffer() = default;
    ByteBuffer(const std::vector<uint8_t>& data) : Buffer(data), ReadPos(0) {}
    ByteBuffer(const uint8_t* data, size_t size) {
        Buffer.assign(data, data + size);  // 내부 vector에 복사
        ReadPos = 0;
    }

    // Write generic type
    template <typename T>
    void Write(const T& data) {
        const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&data);
        Buffer.insert(Buffer.end(), ptr, ptr + sizeof(T));
    }

    // Read generic type
    template <typename T>
    void Read(T& data) {
        if (ReadPos + sizeof(T) > Buffer.size()) throw std::out_of_range("ByteBuffer read overflow");
        std::memcpy(&data, Buffer.data() + ReadPos, sizeof(T));
        ReadPos += sizeof(T);
    }

    // Write string with length prefix
    void WriteString(const std::string& str) {
        uint16_t len = static_cast<uint16_t>(str.length());
        Write(len);
        Buffer.insert(Buffer.end(), str.begin(), str.end());
    }

    // Read string with length prefix
    void ReadString(std::string& str) {
        uint16_t len;
        Read(len);
        if (ReadPos + len > Buffer.size()) throw std::out_of_range("ByteBuffer string read overflow");
        str.assign(Buffer.begin() + ReadPos, Buffer.begin() + ReadPos + len);
        ReadPos += len;
    }

    const std::vector<uint8_t>& GetData() const { return Buffer; }

    void Clear() {
        Buffer.clear();
        ReadPos = 0;
    }

    size_t GetReadPos() const { return ReadPos; }
    void SetReadPos(size_t pos) {
        if (pos > Buffer.size()) throw std::out_of_range("ByteBuffer set read position overflow");
        ReadPos = pos;
    }

    size_t GetSize() const { return Buffer.size(); }
};
