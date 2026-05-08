#include "OrderRepository.h"
#include "../lib/json.hpp"
#include <fstream>
#include <filesystem>

using json = nlohmann::json;
const std::string OrderRepository::kFilePath = "data/orders.json";

OrderRepository::OrderRepository() {
    std::filesystem::create_directories("data");
    load();
}

void OrderRepository::load() {
    std::ifstream f(kFilePath);
    if (!f.is_open()) return;
    auto j = json::parse(f);
    nextId_ = j.value("nextId", 1);
    for (const auto& item : j["orders"])
        orders_.push_back({
            item["id"],
            item["sampleId"],
            item["quantity"],
            statusFromString(item["status"])
        });
}

void OrderRepository::save() const {
    json arr = json::array();
    for (const auto& o : orders_)
        arr.push_back({
            {"id",       o.id},
            {"sampleId", o.sampleId},
            {"quantity", o.quantity},
            {"status",   toString(o.status)}
        });
    std::ofstream f(kFilePath);
    f << json{ {"nextId", nextId_}, {"orders", arr} }.dump(2);
}

void OrderRepository::add(Order& o) {
    o.id = nextId_++;
    orders_.push_back(o);
    save();
}

Order* OrderRepository::findById(int id) {
    for (auto& o : orders_)
        if (o.id == id) return &o;
    return nullptr;
}

const std::vector<Order>& OrderRepository::getAll() const { return orders_; }
