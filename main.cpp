#include <nlohmann/json.hpp>

#include "include/deribitClient.hpp"
#include "include/connection.hpp"

#include <iostream>
#include <fstream>
#include <thread>

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
    std::cout << "q. Exit\n";
}

void instruments(){
    std::cout << "\nSelect an instrument:\n";
    std::cout << "1. Spot\n";
    std::cout << "2. Futures\n";
    std::cout << "3. Options\n";
    std::cout << "q. Exit\n";
}

void placeOrder(){
    bool exit = 0;
    while(!exit){
        instruments();

        char type;
        std::cin >> type;
        switch(type){
            case 'q':
                exit = 1;
                break;
            case '1':
                break;
            case '2':
                break;
            case '3':
                break;
            default:
                std::cout << "\n\nInvalid input!\n\nTry again...\n\n";
                break;
        }
    }
}

void cancelOrder(){
    std::string orderId;

    std::cout << "Enter Order ID: \n";
    std::cin >> orderId;

    if(client.cancelOrder(orderId, response)==ERRNO){
        wc.auth(clientId, clientSecret);
        client.setAccessToken(wc.getAccessToken());
    }

    client.cancelOrder(orderId, response);

    std::cout << "Response: " << response << "\n";
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
                break;
            case '4':
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

    std::thread authThread([&]() {
        while (!done) {
            if (tok.isExpired()) {
                wc.auth(clientId, clientSecret);
                client.setAccessToken(wc.getAccessToken());
            }
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    });

    client = DeribitClient(clientId, clientSecret, wc.getAccessToken(), tok);

    menu();

    std::this_thread::sleep_for(std::chrono::seconds(2));
    wc.disconnect();
    std::this_thread::sleep_for(std::chrono::seconds(2));

    authThread.join();
    connectionThread.join();

    return 0;
}