#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "order.hpp"
#include "token.hpp"

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

#include <openssl/ssl.h>

#include <nlohmann/json.hpp>

#include <iostream>

#define ERRNO 69

using json = nlohmann::json;
using websocketpp::connection_hdl;

using namespace nlohmann::literals;

typedef websocketpp::client<websocketpp::config::asio_tls_client> wsclient;

class Connection{
private:
    wsclient wsClient;
    websocketpp::connection_hdl hdl;

    std::shared_ptr<websocketpp::lib::asio::ssl::context> on_tls_init(connection_hdl hdl);

    void on_open(connection_hdl hdl);
    void on_close(connection_hdl hdl);
    void on_fail(connection_hdl hdl);
    void on_message(connection_hdl hdl, wsclient::message_ptr msg);

    std::string access_token;
    
    Token* tok;

    bool isStreaming;

public:
    Connection(){
        wsClient.clear_access_channels(websocketpp::log::alevel::all);
        wsClient.set_access_channels(websocketpp::log::alevel::app);
        wsClient.init_asio();

        wsClient.set_tls_init_handler(bind(&Connection::on_tls_init, this, std::placeholders::_1));

        wsClient.set_open_handler(bind(&Connection::on_open, this, std::placeholders::_1)); 
        wsClient.set_close_handler(bind(&Connection::on_close, this, std::placeholders::_1));
        wsClient.set_fail_handler(bind(&Connection::on_fail, this, std::placeholders::_1));
        wsClient.set_message_handler(bind(&Connection::on_message, this, std::placeholders::_1, std::placeholders::_2));

        isStreaming = false;
    }

    void initializeToken(Token* tok);

    void connect(const std::string& uri);
    void disconnect();
    void send_message(const std::string& message);
    void auth(const std::string& clientId, const std::string& clientSecret);

    void refreshToken(const std::string& clientId, const std::string& clientSecret);

    std::string getAccessToken();

    int placeOrder(const Order& params, const int& type);
    int cancelOrder(const std::string& orderId);
    int modifyOrder(const std::string& orderId, const double& amount, const double& price);

    void getOrderBook(const std::string& instrument_name, const int& depth);

    int viewCurrentPositions(const std::string& instrument_name);

    int streamSubscriptions(const std::vector<std::string>& connections);
};

#endif