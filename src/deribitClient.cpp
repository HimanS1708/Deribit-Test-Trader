#include "../include/deribitClient.hpp"

DeribitClient::DeribitClient(std::string clientId, std::string clientSecret, std::string accessToken){
    this->clientId = clientId;
    this->clientSecret = clientSecret;
    this->accessToken = accessToken;

    this->subscriptions.clear();
}

std::string DeribitClient::getClientId(){
    return this->clientId;
}

std::string DeribitClient::getClientSecret(){
    return this->clientSecret;
}

void DeribitClient::setAccessToken(std::string access_token){
    this->accessToken = access_token;
}

void DeribitClient::addSubscriptions(std::vector<std::string> subscriptions){
    for(const std::string x:subscriptions){
        this->subscriptions.push_back("deribit_price_index." + x);
    }
}

std::vector<std::string> DeribitClient::getSubscriptions(){
    return this->subscriptions;
}
