#pragma once

#include <functional>
#include <string>

#include "MessageEvent.h"

class WebSocket
{
  public:
    virtual void close() = 0;

    virtual int getReadyState() = 0;

    virtual std::function<void(MessageEvent)> getOnMessage() = 0;
    virtual void setOnMessage(std::function<void(MessageEvent)>) = 0;

    virtual void send(std::string const &) = 0;
};
