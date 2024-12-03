#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include <chrono>

class Token{
private:
    std::string accessToken;
    std::chrono::system_clock::time_point expiryTime;

public:
    void setAccessToken(std::string accessToken);
    void setExpiryTime(std::chrono::system_clock::time_point expiryTime);

    bool isExpired();
};

#endif
