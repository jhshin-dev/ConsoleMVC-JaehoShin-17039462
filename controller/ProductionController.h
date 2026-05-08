#pragma once
#include "../model/SampleRepository.h"
#include "../model/OrderRepository.h"

class ProductionController {
public:
    ProductionController(SampleRepository& sampleRepo, OrderRepository& orderRepo);
    void run();
private:
    SampleRepository& sampleRepo_;
    OrderRepository&  orderRepo_;
    void registerSample();
    void listSamples();
    void approveOrder();
    void rejectOrder();
    void listProduction();
    void completeProduction();
};
