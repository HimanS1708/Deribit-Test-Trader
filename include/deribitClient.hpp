#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

#define MAX_ORDERS 100

class DeribitClient{
private:
    std::string clientId;
    std::string clientSecret;
    std::string accessToken;

public:
    DeribitClient() = default;
    DeribitClient(std::string clientId, std::string clientSecret, std::string accessToken);
    
    std::string getClientId();
    std::string getClientSecret();

    void setAccessToken(std::string access_token);

};

#endif
