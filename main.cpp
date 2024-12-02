#include <nlohmann/json.hpp>

#include "include/deribitClient.hpp"
#include "include/connection.hpp"

#include <iostream>
#include <fstream>
#include <thread>

using json = nlohmann::json;

std::string clientId="";
std::string clientSecret="";

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
    std::cout << "2. Modify Order\n";
    std::cout << "3. Cancel Order\n";
    std::cout << "4. Get Order Book\n";
    std::cout << "5. View Current Positions\n";
    std::cout << "6. Subscribe to symbols\n";
    std::cout << "7. Stream orderbook updates for subscribed symbols\n";
    std::cout << "q. Exit\n";
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
                break;
            case '1':
                break;
            case '2':
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

    DeribitClient client;

    client.setClientId(clientId);
    client.setClientSecret(clientSecret);

    Connection wc;

    std::string uri = "wss://test.deribit.com/ws/api/v2/";

    std::thread connectionThread([&]() {
        wc.connect(uri);
    });
    std::this_thread::sleep_for(std::chrono::seconds(2));

    menu();

    wc.disconnect();
    std::this_thread::sleep_for(std::chrono::seconds(2));

    connectionThread.join();

    return 0;
}