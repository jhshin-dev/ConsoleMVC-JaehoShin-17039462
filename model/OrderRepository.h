#pragma once
#include <vector>
#include <string>
#include "Order.h"

class OrderRepository {
public:
    OrderRepository();
    void add(Order& o);
    Order* findById(int id);
    const std::vector<Order>& getAll() const;
    void save() const;
private:
    std::vector<Order> orders_;
    int nextId_ = 1;
    void load();
    static const std::string kFilePath;
};
