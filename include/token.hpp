#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>
#include <chrono>

class Token{
private:
    std::string accessToken;
    std::time_t expiryTime;

public:
    void setAccessToken(std::string accessToken);
    void setExpiryTime(std::time_t expiryTime);

    std::time_t getExpiryTime();

    bool isExpired();
};

#endif
