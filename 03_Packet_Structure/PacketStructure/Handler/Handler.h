#pragma once

#include <string>
#include <winsock2.h>

class IHandler 
{
    public:
        virtual void Handle(SOCKET socket, std::shared_ptr<void> dto) = 0;

        virtual ~IHandler() = default;
};

template <typename T>
class Handler : public IHandler
{
    public:
        void Handle(SOCKET socket, std::shared_ptr<void> data) override {
            auto dto = std::static_pointer_cast<T>(data);
            HandleImpl(socket, *dto);
        }

        virtual ~Handler() = default;

        virtual void HandleImpl(SOCKET socket, const T& dto) = 0;
        
};