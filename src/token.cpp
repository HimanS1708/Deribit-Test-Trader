#include "../include/token.hpp"

void Token::setAccessToken(const std::string& accessToken){
    this->accessToken = accessToken;
}

void Token::setExpiryTime(const std::time_t& expiryTime){
    this->expiryTime = expiryTime;
}

std::time_t Token::getExpiryTime(){
    return this->expiryTime;
}

bool Token::isExpired(){
    return std::time(NULL) >= this->expiryTime;
}
