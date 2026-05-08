#pragma once
#include <vector>
#include <string>
#include "../model/Sample.h"
#include "../model/Order.h"

class ProductionView {
public:
    static int         readMenuChoice();
    static std::string readSampleName();
    static int         readInitialStock();
    static void        showSamples(const std::vector<Sample>& samples);
    static void        showReservedOrders(const std::vector<Order>& orders, const std::vector<Sample>& samples);
    static void        showProducingOrders(const std::vector<Order>& orders, const std::vector<Sample>& samples);
    static int         readOrderId();
    static void        showSampleRegistered(const Sample& s);
    static void        showApproved(const Order& o);
    static void        showRejected(const Order& o);
    static void        showCompleted(const Order& o);
    static void        showError(const std::string& msg);
};
