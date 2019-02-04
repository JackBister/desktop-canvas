#include "WebSocketpp.h"

#include "../Logger/Logger.h"

static auto logger = Logger::get();

static void runClient(WebSocketpp * ws) {}

WebSocketpp::WebSocketpp(std::string const & url)
{
  using websocketpp::lib::placeholders::_1;

  try {
    client.clear_access_channels(websocketpp::log::alevel::all);
    client.clear_error_channels(websocketpp::log::elevel::all);

    websocketpp::lib::error_code ec;

    client.init_asio();
    client.start_perpetual();
    thread = std::thread(&Client::run, &client);

    auto connection = client.get_connection(url, ec);

    if (ec) {
      logger->info("Error when connecting using websocketpp %s", ec.message().c_str());
    }

    connection->set_open_handler([this](websocketpp::connection_hdl) { this->readyState = 1; });

    connection->set_fail_handler([this](websocketpp::connection_hdl) { this->readyState = 3; });

    connection->set_close_handler([this](websocketpp::connection_hdl) { this->readyState = 3; });

    client.connect(connection);
    logger->info("post connect");

    connectionHandle = connection->get_handle();
    logger->info("constructor end");

  } catch (websocketpp::exception const & e) {
    std::cout << e.what() << std::endl;
  }
}

void WebSocketpp::close()
{
  websocketpp::lib::error_code ec;
  client.close(connectionHandle, websocketpp::close::status::normal, "", ec);
  if (ec) {
    logger->info("Error while closing websocketpp %s", ec.message().c_str());
  }
}

int WebSocketpp::getReadyState()
{
  return readyState;
}

std::function<void(MessageEvent)> WebSocketpp::getOnMessage()
{
  return handler;
}

void WebSocketpp::setOnMessage(std::function<void(MessageEvent)> handler)
{
  this->handler = handler;
  try {
    auto con = client.get_con_from_hdl(connectionHandle);
    con->set_message_handler([this](websocketpp::connection_hdl, Client::message_ptr message) {
      if (message->get_opcode() != websocketpp::frame::opcode::TEXT) {
        logger->info("Unhandled WebSocket opcode! %d", message->get_opcode());
        return;
      }
      this->handler({message->get_payload()});
    });
  } catch (websocketpp::exception const & e) {
    std::cout << e.what() << std::endl;
  }
}

void WebSocketpp::send(std::string const & message)
{
  websocketpp::lib::error_code ec;
  client.send(connectionHandle, message, websocketpp::frame::opcode::TEXT, ec);
  if (ec) {
    logger->info("Error while sending websocketpp message %s", ec.message().c_str());
  }
}
