#include "../include/deribitClient.hpp"

std::string DeribitClient::getClientId(){
    return this->clientId;
}

std::string DeribitClient::getClientSecret(){
    return this->clientSecret;
}

void DeribitClient::setClientId(std::string clientId){
    this->clientId = clientId;
}

void DeribitClient::setClientSecret(std::string clientSecret){
    this->clientSecret = clientSecret;
}