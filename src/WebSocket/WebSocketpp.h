#pragma once
#include "WebSocket.h"

#include <thread>

#ifdef _MSC_VER
#include <malloc.h>
#define alloca _alloca
#endif
#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_TYPE_TRAITS_
#define _WEBSOCKETPP_CPP11_RANDOM_DEVICE_
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

class WebSocketpp : public WebSocket {
public:
	WebSocketpp(std::string const& url);

	virtual void close() override;
	virtual int getReadyState() override;
	virtual std::function<void(MessageEvent)> getOnMessage() override;
	virtual void setOnMessage(std::function<void(MessageEvent)>) override;
	virtual void send(std::string const &) override;

private:
	using Client = websocketpp::client<websocketpp::config::asio_client>;

	Client client;
	websocketpp::connection_hdl connectionHandle;
	std::function<void(MessageEvent)> handler;
	int readyState = 0;
	std::thread thread;
};
