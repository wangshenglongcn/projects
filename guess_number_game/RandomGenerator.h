#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <random>

class RandomGenerator {
private:
    std::random_device rd;
    std::mt19937 gen;
public:
    RandomGenerator() : gen(rd()) {}

    int getRandom(int start, int end) {
        std::uniform_int_distribution<int> distribution(start, end);
        return distribution(gen);
    }
};

#endif // RANDOMGENERATOR_H
