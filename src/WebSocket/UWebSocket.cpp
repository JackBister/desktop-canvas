#include "UWebSocket.h"

#include "../Logger/Logger.h"

using UWSClient = uWS::WebSocket<uWS::CLIENT>;

static auto logger = ILogger::get();

UWebSocket::UWebSocket(std::string const & url)
{
	hub.onConnection([this](UWSClient * ws, uWS::HttpRequest req) {
		this->readyState = 1;
		this->ws = ws;
	});
	hub.onDisconnection([this](UWSClient * ws, int code, char * message, size_t length) {
		this->readyState = 3;
	});

	hub.connect(url);

	thread = std::thread([this]() {
		hub.run();
	});
}

void UWebSocket::close()
{
	hub.getDefaultGroup<uWS::CLIENT>().close();
}

int UWebSocket::getReadyState()
{
	return readyState;
}

std::function<void(MessageEvent)> UWebSocket::getOnMessage()
{
	return handler;
}

void UWebSocket::setOnMessage(std::function<void(MessageEvent)> handler)
{
	this->handler = handler;
	hub.onMessage([handler](UWSClient * ws, char * msg, size_t length, uWS::OpCode opcode) {
		if (opcode != uWS::OpCode::TEXT) {
			logger->info("Unhandled WebSocket opcode! %d", opcode);
			return;
		}
		std::string message(msg, length);
		handler({message});
	});
}

void UWebSocket::send(std::string const& msg)
{
	ws->send(msg.c_str());
}
