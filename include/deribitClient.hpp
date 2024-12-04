#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>

class DeribitClient{
private:
    std::string clientId;
    std::string clientSecret;
    std::string accessToken;
    std::vector<std::string> subscriptions;

public:
    DeribitClient() = default;
    DeribitClient(std::string clientId, std::string clientSecret, std::string accessToken);
    
    std::string getClientId();
    std::string getClientSecret();

    void setAccessToken(std::string access_token);

    void addSubscriptions(std::vector<std::string> subscriptions);
    std::vector<std::string> getSubscriptions();
};

#endif
