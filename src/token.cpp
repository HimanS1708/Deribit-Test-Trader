#include "../include/token.hpp"

void Token::setAccessToken(std::string accessToken){
    this->accessToken = accessToken;
}

void Token::setExpiryTime(std::chrono::system_clock::time_point expiryTime){
    this->expiryTime = expiryTime;
}

bool Token::isExpired(){
    return std::chrono::system_clock::now() >= this->expiryTime;
}
