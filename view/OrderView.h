#pragma once
#include <vector>
#include <string>
#include "../model/Order.h"
#include "../model/Sample.h"

class OrderView {
public:
    static int  readMenuChoice();
    static void showSamples(const std::vector<Sample>& samples);
    static int  readSampleId();
    static int  readQuantity();
    static void showOrders(const std::vector<Order>& orders, const std::vector<Sample>& samples);
    static void showConfirmedOrders(const std::vector<Order>& orders, const std::vector<Sample>& samples);
    static int  readOrderId();
    static void showOrderRegistered(const Order& o);
    static void showReleased(const Order& o);
    static void showError(const std::string& msg);
};
