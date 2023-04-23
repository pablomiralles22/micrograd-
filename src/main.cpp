#include <Engine.hpp>
#include <iostream>

int main() {
    micrograd::engine::Value<float> x1(2.), x2(4.);
    std::cout << micrograd::engine::logistic(x1 * x2) << std::endl;
    std::cout << x1 / x2 << std::endl;
    std::cout << (x1 ^ x2) << std::endl;
    std::cout << micrograd::engine::relu(-x1) << std::endl;
}
