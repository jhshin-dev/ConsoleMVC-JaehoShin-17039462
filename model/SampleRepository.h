#pragma once
#include <vector>
#include <string>
#include "Sample.h"

class SampleRepository {
public:
    SampleRepository();
    void add(Sample& s);
    Sample* findById(int id);
    const std::vector<Sample>& getAll() const;
    void save() const;
private:
    std::vector<Sample> samples_;
    int nextId_ = 1;
    void load();
    static const std::string kFilePath;
};
