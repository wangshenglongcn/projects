#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <random>

// 封装随机数生成器，生成指定范围内的随机整数
class RandomGenerator {
private:
    std::random_device rd;  // 真随机数种子
    std::mt19937 gen;  // 梅森旋转引擎
public:
    // 构造函数：用随机设备初始化生成器
    RandomGenerator() : gen(rd()) {}

    // 获取指定范围内的随机整数
    int getRandom(int start, int end) {
        std::uniform_int_distribution<int> distribution(start, end);
        return distribution(gen);
    }
};

#endif // RANDOMGENERATOR_H
