#include "../include/deribitClient.hpp"

DeribitClient::DeribitClient(std::string clientId, std::string clientSecret, std::string accessToken){
    this->clientId = clientId;
    this->clientSecret = clientSecret;
    this->accessToken = accessToken;
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
