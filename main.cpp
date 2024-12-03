#include <nlohmann/json.hpp>

#include "include/deribitClient.hpp"
#include "include/connection.hpp"

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

using json = nlohmann::json;

std::string clientId = "";
std::string clientSecret = "";
std::string response = "";

DeribitClient client;
Connection wc;
Token tok;

bool done = false;

void welcomeHeader(){
    std::cout << "\n\n\n =======================================================================================\n\n\n";
    std::cout << "\t\t Welcome to Deribit Test Trader! \n\n\n";
    std::cout << "=======================================================================================\n\n\n";
}

void fetchApiKeys(){
    std::ifstream keys("api_keys.json");
    json data = json::parse(keys);
    clientId = data["clientId"];
    clientSecret = data["clientSecret"];
}

void options(){
    std::cout << "\nSelect an action:\n";
    std::cout << "1. Place Order\n";
    std::cout << "2. Cancel Order\n";
    std::cout << "3. Modify Order\n";
    std::cout << "4. Get Order Book\n";
    std::cout << "5. View Current Positions\n";
    std::cout << "6. Subscribe to symbols\n";
    std::cout << "7. Stream orderbook updates for subscribed symbols\n";
    std::cout << "q. Exit\n\n\n";
}

void placeOrder(){
    Order params;

    std::cout << "Enter instrument name: (press q to exit) ";
    std::cin >> params.instrument_name;
    if(params.instrument_name == "q")
        return;
    std::cout << "\nEnter amount: ";
    std::cin >> params.amount;
    std::cout << "\nEnter type: (limit or market) ";
    std::cin >> params.type;
    if(params.type == "limit"){
        std::cout << "\nEnter price: ";
        std::cin >> params.price;
    }
    std::cout << "\nEnter label: ";
    std::cin >> params.label;
    std::cout << "\nEnter side: (1 for buy/2 for sell) ";
    int side;
    std::cin >> side;

    std::cout << "\n\n\n";

    if(wc.placeOrder(params, side) == ERRNO){
        wc.auth(clientId, clientSecret);
        client.setAccessToken(wc.getAccessToken());
        wc.placeOrder(params, side);
    }


    std::cout << "\n\n\n";
}

void cancelOrder(){
    std::string orderId;

    std::cout << "\nEnter Order ID: ";
    std::cin >> orderId;

    std::cout << "\n\n\n";

    if(wc.cancelOrder(orderId) == ERRNO){
        wc.auth(clientId, clientSecret);
        client.setAccessToken(wc.getAccessToken());
        wc.cancelOrder(orderId);
    }


    std::cout << "\n\n\n";
}

void modifyOrder(){
    std::string orderId;
    double amount;
    double price;

    std::cout << "\nEnter Order ID: ";
    std::cin >> orderId;

    std::cout << "\nEnter amount: ";
    std::cin >> amount;

    std::cout << "\nEnter price: ";
    std::cin >> price;
    
    std::cout << "\n\n\n";

    if(wc.modifyOrder(orderId, amount, price) == ERRNO){
        wc.auth(clientId, clientSecret);
        client.setAccessToken(wc.getAccessToken());
        wc.modifyOrder(orderId, amount, price);
    }

    std::cout << "\n\n\n";
}

void getOrderBook(){
    std::string instrument_name;
    int depth;

    std::cout << "\nEnter Instrument Name: ";
    std::cin >> instrument_name;

    std::cout << "\nEnter depth: ";
    std::cin >> depth;

    std::cout << "\n\n\n";

    wc.getOrderBook(instrument_name, depth);

    std::cout << "\n\n\n";
}

void menu(){
    bool exit=0;

    while(!exit){
        options();
        
        char type;
        std::cin >> type;
        switch(type){
            case 'q':
                exit = 1;
                done = 1;
                break;
            case '1':
                placeOrder();
                break;
            case '2':
                cancelOrder();
                break;
            case '3':
                modifyOrder();
                break;
            case '4':
                getOrderBook();
                break;
            case '5':
                break;
            case '6':
                break;
            case '7':
                break;
            default:
                std::cout << "\n\nInvalid input!\n\nTry again...\n\n";
                break;
        }
    }
}

int main(){
    welcomeHeader();

    fetchApiKeys();

    std::string uri = "wss://test.deribit.com/ws/api/v2/";

    std::thread connectionThread([&]() {
        wc.connect(uri);
    });
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    wc.initializeToken(&tok);

    std::thread authThread([&]() {
        while (!done) {
            if (tok.isExpired()) {
                wc.auth(clientId, clientSecret);
                client.setAccessToken(wc.getAccessToken());
            }
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    });

    client = DeribitClient(clientId, clientSecret, wc.getAccessToken());

    menu();

    std::this_thread::sleep_for(std::chrono::seconds(2));
    wc.disconnect();
    std::this_thread::sleep_for(std::chrono::seconds(2));

    authThread.join();
    connectionThread.join();

    return 0;
}