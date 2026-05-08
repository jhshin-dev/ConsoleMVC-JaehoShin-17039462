#pragma once
#include "../model/SampleRepository.h"
#include "../model/OrderRepository.h"

class OrderController {
public:
    OrderController(SampleRepository& sampleRepo, OrderRepository& orderRepo);
    void run();
private:
    SampleRepository& sampleRepo_;
    OrderRepository&  orderRepo_;
    void registerOrder();
    void listOrders();
    void processRelease();
};
