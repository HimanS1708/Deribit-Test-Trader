#include <nlohmann/json.hpp>

#include "../include/connection.hpp"
#include "../include/deribitClient.hpp"
#include "../include/token.hpp"

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

class Timer{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;

public:
    Timer(){
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer(){
        std::chrono::time_point<std::chrono::high_resolution_clock> m_endTimepoint = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(m_endTimepoint).time_since_epoch().count();

        auto duration = end - start;
        double ms = duration * 0.001;

        std::cout << duration << "us (" << ms << "ms)\n\n";
    }
};

std::string clientId;
std::string clientSecret;

DeribitClient client;
Connection wc;
Token tok;

bool done = false;

void fetchApiKeys(){
    std::ifstream keys("api_keys.json");
    json data = json::parse(keys);
    clientId = data["clientId"];
    clientSecret = data["clientSecret"];
}

int main(){
    fetchApiKeys();

    std::string uri = "wss://test.deribit.com/ws/api/v2/";

    std::thread connectionThread([&]() {
        wc.connect(uri);
    });
    
    wc.initializeToken(&tok);

    std::this_thread::sleep_for(std::chrono::seconds(1));

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

    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Place Order benchmarks: \n\n";
    {
        Order params;
        params.instrument_name = "ETH-PERPETUAL";
        params.amount = 1;
        params.type = "limit";
        params.price = 2500;
        params.label = "limit000239";
        Timer timer;
        wc.placeOrder(params, 1);
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Modify Order benchmarks: \n";
    {
        std::string orderId;

        std::cout << "Enter Order ID: \n\n";
        std::cin >> orderId;

        Timer timer;
        wc.modifyOrder(orderId, 1, 3000);
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Cancel Order benchmarks: \n";
    {
        std::string orderId;

        std::cout << "Enter Order ID: \n\n";
        std::cin >> orderId;

        Timer timer;
        wc.cancelOrder(orderId);
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Get Order Book benchmarks: \n\n";
    {
        Timer timer;
        wc.getOrderBook("BTC-PERPETUAL", 10);
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "\n\nView Current Position benchmarks: \n\n";
    {
        Timer timer;
        wc.viewCurrentPositions("ETH-PERPETUAL");
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));

    done = 1;

    std::this_thread::sleep_for(std::chrono::seconds(1));
    wc.disconnect();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    authThread.join();
    connectionThread.join();

    return 0;
}
