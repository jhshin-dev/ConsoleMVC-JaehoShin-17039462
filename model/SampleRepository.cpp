#include "SampleRepository.h"
#include "../lib/json.hpp"
#include <fstream>
#include <filesystem>

using json = nlohmann::json;
const std::string SampleRepository::kFilePath = "data/samples.json";

SampleRepository::SampleRepository() {
    std::filesystem::create_directories("data");
    load();
}

void SampleRepository::load() {
    std::ifstream f(kFilePath);
    if (!f.is_open()) return;
    auto j = json::parse(f);
    nextId_ = j.value("nextId", 1);
    for (const auto& item : j["samples"])
        samples_.push_back({ item["id"], item["name"], item["stock"] });
}

void SampleRepository::save() const {
    json arr = json::array();
    for (const auto& s : samples_)
        arr.push_back({ {"id", s.id}, {"name", s.name}, {"stock", s.stock} });
    std::ofstream f(kFilePath);
    f << json{ {"nextId", nextId_}, {"samples", arr} }.dump(2);
}

void SampleRepository::add(Sample& s) {
    s.id = nextId_++;
    samples_.push_back(s);
    save();
}

Sample* SampleRepository::findById(int id) {
    for (auto& s : samples_)
        if (s.id == id) return &s;
    return nullptr;
}

const std::vector<Sample>& SampleRepository::getAll() const { return samples_; }
