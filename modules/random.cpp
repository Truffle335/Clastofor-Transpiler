#pragma once
#include <random>
#include <string>

namespace ClsfModules {

class RandomModule {
    std::mt19937 gen;

public:
    RandomModule() : gen(std::random_device{}()) {}

    int randomInt(int min, int max) {
        std::uniform_int_distribution<> dis(min, max);
        return dis(gen);
    }

    float randomFloat(float min, float max) {
        std::uniform_real_distribution<> dis(min, max);
        return dis(gen);
    }

    bool randomBool() {
        std::uniform_int_distribution<> dis(0, 1);
        return dis(gen) == 1;
    }
};

} // namespace ClsfModules
