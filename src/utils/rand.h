#pragma once

#include <random>
#include <iostream>

namespace Random {
    
    double GetRandomDoubleUniform(double min, double max, unsigned int seed) {
        static std::default_random_engine generator(seed);
        std::uniform_real_distribution<double> distribution(min, max);
        return distribution(generator);
    }

    double GetRandomDoubleNormal(double stddev, double average, unsigned int seed) {
        static std::default_random_engine generator(seed);
        std::normal_distribution<double> distribution(average, stddev);
        return distribution(generator);
    }

} // namespace Random
