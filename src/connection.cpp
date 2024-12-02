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

void Connection::on_message(connection_hdl hdl, client::message_ptr msg){
    json received_json = json::parse(msg->get_payload());

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
    client::connection_ptr con = wsClient.get_connection(uri, ec);

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
