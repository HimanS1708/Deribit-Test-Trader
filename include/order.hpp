#ifndef ORDER_HPP
#define ORDER_HPP

#include <iostream>

struct Order{
    std::string instrument_name;
    double amount;
    double price;
    std::string type;
    std::string label;
};

#endif