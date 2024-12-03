#include "../include/connection.hpp"

void Connection::on_open(connection_hdl hdl){
    this->hdl = hdl;
    std::cout << "Connected!\n";
}

void Connection::on_close(connection_hdl hdl){
    std::cout << "Connection closed!\n";
}

void Connection::on_fail(connection_hdl hdl){
    std::cout << "Connection failed\n";
}

void Connection::on_message(connection_hdl hdl, wsclient::message_ptr msg){
    json received_json = json::parse(msg->get_payload());
    if(received_json.contains("params"))
        std::cout << "\nReceived: " << received_json.dump(4) << "\n\n";
}

std::shared_ptr<websocketpp::lib::asio::ssl::context> Connection::on_tls_init(connection_hdl hdl){
    auto ctx = std::make_shared<websocketpp::lib::asio::ssl::context>(websocketpp::lib::asio::ssl::context::tlsv12);

    try{
        ctx->set_options(websocketpp::lib::asio::ssl::context::default_workarounds |
                            websocketpp::lib::asio::ssl::context::no_sslv2 |
                            websocketpp::lib::asio::ssl::context::no_sslv3 |
                            websocketpp::lib::asio::ssl::context::single_dh_use);
        
    }
    catch(std::exception& e){
        std::cerr << "Error in context pointer: " << e.what() << "\n";
    }

    return ctx;
}

void Connection::connect(const std::string& uri){
    websocketpp::lib::error_code ec;
    wsclient::connection_ptr con = wsClient.get_connection(uri, ec);

    if(ec){
        std::cout << "Could not create connection because: " << ec.message() << "\n";
        return;
    }

    wsClient.connect(con);
    wsClient.run();
}

void Connection::disconnect(){
    wsClient.stop();
}

void Connection::send_message(const std::string& message){
    websocketpp::lib::error_code ec;
    wsClient.send(this->hdl, message, websocketpp::frame::opcode::text, ec);

    if(ec){
        std::cout << "Send failed because: " << ec.message() << "\n";
    }
}

void Connection::auth(std::string clientId, std::string clientSecret){
    json authReq = {
        {"jsonrpc", "2.0"},
        {"id", 9929},
        {"method", "public/auth"},
        {"params", {
            {"grant_type", "client_credentials"},
            {
            "client_id", clientId},
            {"client_secret", clientSecret}
        }}
    };

    send_message(authReq.dump());
}

void Connection::refreshToken(std::string clientId, std::string clientSecret){
    Connection::auth(clientId, clientSecret);
}

std::string Connection::getAccessToken(){
    return this->access_token;
}
