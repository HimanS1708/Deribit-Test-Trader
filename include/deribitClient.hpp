#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class DeribitClient{
private:
    std::string clientId;
    std::string clientSecret;

public:
    std::string getClientId();
    std::string getClientSecret();
    void setClientId(std::string clientId);
    void setClientSecret(std::string clientSecret);
};

#endif
