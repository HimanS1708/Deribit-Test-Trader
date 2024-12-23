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
    if(received_json.contains("result") && received_json["result"].contains("access_token")){
        tok->setAccessToken(received_json["result"]["access_token"]);
        std::time_t t = received_json["result"]["expires_in"];
        tok->setExpiryTime(t + std::time(NULL));
    }
    else if(received_json.contains("method") && received_json["method"] == "subscription" && isStreaming){
        system("cls");
        std::cout << "(Press q to stop)\n\n" << received_json.dump(2) << std::endl;
        if(GetAsyncKeyState('Q') & 0x0001) {
            system("cls");
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
            isStreaming = false;
            json unsubscribe = {
                {"jsonrpc", "2.0"},
                {"id", 154},
                {"method", "private/unsubscribe_all"},
                {"params", {

                }}
            };

            send_message(unsubscribe.dump());
        }
    }
    else if(received_json.contains("id") && received_json["id"] == 5275){
        std::cout << "Order placed for " << received_json["result"]["order"]["instrument_name"] << " at price " << received_json["result"]["order"]["price"] << " (Order Id: " << received_json["result"]["order"]["order_id"] << ")\n\n";
    }
    else if(received_json.contains("id") && received_json["id"] == 4214){
        std::cout << "Order for " << received_json["result"]["instrument_name"] << " cancelled.\n\n";
    }
    else if(received_json.contains("id") && received_json["id"] == 3725){
        std::cout << "Order placed for " << received_json["result"]["order"]["instrument_name"] << " at price " << received_json["result"]["order"]["price"] << " and amount " << received_json["result"]["order"]["amount"] << "\n\n";
    }
    else if(received_json.contains("id") && received_json["id"] != 154)
        std::cout << received_json.dump(2) << "\n";
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

void Connection::initializeToken(Token* tok){
    this->tok = tok;
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

void Connection::auth(const std::string& clientId, const std::string& clientSecret){
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

void Connection::refreshToken(const std::string& clientId, const std::string& clientSecret){
    Connection::auth(clientId, clientSecret);
}

std::string Connection::getAccessToken(){
    return this->access_token;
}

int Connection::placeOrder(const Order& params, const int& type){
    if(tok->isExpired()){
        return ERRNO;
    }
    
    std::string action = "";
    if(type == 1){
        action = "buy";
    }
    else if(type == 2){
        action = "sell";
    }

    json arguments;
    arguments["instrument_name"] = params.instrument_name;
    arguments["amount"] = params.amount;
    arguments["type"] = params.type;
    if(params.type == "limit"){
        arguments["price"] = params.price;
    }
    arguments["label"] = params.label;

    json order = {
        {"jsonrpc", "2.0"},
        {"id", 5275},
        {"method", "private/" + action},
        {"params", arguments}
    };
    send_message(order.dump());

    return 0;
}

int Connection::cancelOrder(const std::string& orderId){
    if(tok->isExpired()){
        return ERRNO;
    }

    json jId;
    jId["order_id"] = orderId;

    json cancel = {
        {"jsonrpc", "2.0"},
        {"id", 4214},
        {"method", "private/cancel"},
        {"params", jId}
    };
    send_message(cancel.dump());
    return 0;
}

int Connection::modifyOrder(const std::string& orderId, const double& amount, const double& price){
    if(tok->isExpired()){
        return ERRNO;
    }

    json arguments;
    arguments["order_id"] = orderId;

    if(amount != -1){
        arguments["amount"] = amount;
    }

    if(price != -1){
        arguments["price"] = price;
    }

    json modify = {
        {"jsonrpc", "2.0"},
        {"id", 3725},
        {"method", "private/edit"},
        {"params", arguments}
    };
    send_message(modify.dump());
    return 0;
}

void Connection::getOrderBook(const std::string& instrument_name, const int& depth){
    json orderBook = {
        {"jsonrpc", "2.0"},
        {"id", 8772},
        {"method", "public/get_order_book"},
        {"params", {
            {"instrument_name", instrument_name},
            {"depth", depth}
        }}
    };
    send_message(orderBook.dump());
}

int Connection::viewCurrentPositions(const std::string& instrument_name){
    if(tok->isExpired()){
        return ERRNO;
    }

    json currentPositions = {
        {"jsonrpc", "2.0"},
        {"id", 404},
        {"method", "private/get_position"},
        {"params", {
            {"instrument_name", instrument_name}
        }}
    };
    send_message(currentPositions.dump());
    return 0;
}

int Connection::streamSubscriptions(const std::vector<std::string>& connections){
    if(tok->isExpired()){
        return ERRNO;
    }

    json subscriptions = connections;

    json subscribe = {
        {"jsonrpc", "2.0"},
        {"id", 4235},
        {"method", "private/subscribe"},
        {"params", {
            {"channels", subscriptions}
        }}
    };
    isStreaming = true;
    send_message(subscribe.dump());
    while(isStreaming);
    return 0;
}
