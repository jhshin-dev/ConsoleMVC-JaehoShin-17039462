#pragma once
#include "OrderStatus.h"

struct Order {
    int id = 0;
    int sampleId = 0;
    int quantity = 0;
    OrderStatus status = OrderStatus::RESERVED;
};
