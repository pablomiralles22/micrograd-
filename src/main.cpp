#include <Engine.hpp>
#include <iostream>

int main() {
    micrograd::engine::Value<float> x1(2.), x2(4.);
    std::cout << exp(x1 * x2) << std::endl;
}
