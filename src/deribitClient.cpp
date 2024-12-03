#include "../include/deribitClient.hpp"

DeribitClient::DeribitClient(std::string clientId, std::string clientSecret, std::string accessToken, Token tok){
    this->clientId = clientId;
    this->clientSecret = clientSecret;
    this->accessToken = accessToken;

    this->tok = tok;
}

std::string DeribitClient::getClientId(){
    return this->clientId;
}

std::string DeribitClient::getClientSecret(){
    return this->clientSecret;
}

int DeribitClient::placeOrder(Order params, int type, std::string response){
    if(tok.isExpired()){
        return ERRNO;
    }
    return 0;
}

int DeribitClient::cancelOrder(std::string orderId, std::string response){
    if(tok.isExpired()){
        return ERRNO;
    }
    return 0;
}

int DeribitClient::modifyOrder(std::string orderId, double amount, double price, std::string advanced){
    if(tok.isExpired()){
        return ERRNO;
    }
    return 0;
}

void DeribitClient::setAccessToken(std::string access_token){
    this->accessToken = access_token;
}
