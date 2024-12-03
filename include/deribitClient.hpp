#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "order.hpp"
#include "token.hpp"

#include <iostream>

#define ERRNO 69

class DeribitClient{
private:
    std::string clientId;
    std::string clientSecret;
    std::string accessToken;
    Token tok;

public:
    DeribitClient() = default;
    DeribitClient(std::string clientId, std::string clientSecret, std::string accessToken, Token tok);
    
    std::string getClientId();
    std::string getClientSecret();
    
    int placeOrder(Order params, int type, std::string response);
    int cancelOrder(std::string orderId, std::string response);
    int modifyOrder(std::string orderId, double amount, double price, std::string advanced);

    void setAccessToken(std::string access_token);
};

#endif
