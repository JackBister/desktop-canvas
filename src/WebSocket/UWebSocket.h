#pragma once

#include "WebSocket.h"

#include <thread>

#include <uWS/uWS.h>

class UWebSocket : public WebSocket {
public:
	UWebSocket(std::string const& url);

	virtual void close() override;
	virtual int getReadyState() override;
	virtual std::function<void(MessageEvent)> getOnMessage() override;
	virtual void setOnMessage(std::function<void(MessageEvent)>) override;
	virtual void send(std::string const &) override;

private:
	uWS::Hub hub;
	uWS::WebSocket<uWS::CLIENT> * ws;
	std::thread thread;

	std::function<void(MessageEvent)> handler;

	int readyState = 0;
};
