#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

#include <openssl/ssl.h>

#include <nlohmann/json.hpp>

#include <iostream>

using json = nlohmann::json;
using websocketpp::connection_hdl;

typedef websocketpp::client<websocketpp::config::asio_tls_client> client;

class Connection{
private:
    client wsClient;
    websocketpp::connection_hdl hdl;

    std::shared_ptr<websocketpp::lib::asio::ssl::context> on_tls_init(connection_hdl hdl);

    void on_open(connection_hdl hdl);
    void on_close(connection_hdl hdl);
    void on_fail(connection_hdl hdl);
    void on_message(connection_hdl hdl, client::message_ptr msg);

public:
    Connection(){
        wsClient.set_access_channels(websocketpp::log::alevel::all);
        wsClient.clear_access_channels(websocketpp::log::alevel::frame_payload);
        wsClient.init_asio();

        wsClient.set_tls_init_handler(bind(&Connection::on_tls_init, this, std::placeholders::_1));

        wsClient.set_open_handler(bind(&Connection::on_open, this, std::placeholders::_1)); 
        wsClient.set_close_handler(bind(&Connection::on_close, this, std::placeholders::_1));
        wsClient.set_fail_handler(bind(&Connection::on_fail, this, std::placeholders::_1));
        wsClient.set_message_handler(bind(&Connection::on_message, this, std::placeholders::_1, std::placeholders::_2));
    }

    void connect(const std::string& uri);
    void disconnect();
};

#endif