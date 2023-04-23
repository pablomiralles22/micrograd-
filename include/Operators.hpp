#ifndef OPERATORS_HPP
#define OPERATORS_HPP

#include <math.h>

#include <concepts>
#include <cstddef>

// Base concepts
namespace micrograd::operators {

template <typename T>
concept Operand = std::convertible_to<T, double>;

template <typename Op, typename T>
concept UnaryOp = Operand<T> && requires(Op op, T x) {
    { op(x) } -> std::convertible_to<T>;
    { op.dx(x) } -> std::convertible_to<T>;
};

template <typename Op, typename T>
concept BinaryOp = Operand<T> && requires(Op op, T x, T y) {
    { op(x, y) } -> std::convertible_to<T>;
    { op.dx(x, y) } -> std::convertible_to<T>;
    { op.dy(x, y) } -> std::convertible_to<T>;
};

// Definition of implemented operators, both unary and binary

template <Operand T>
struct ExponentialOp {
    T operator()(T x) { return exp(x); }
    T dx(T x) { return exp(x); }
};

template <Operand T>
struct ReluOp {
    T operator()(T x) { return x > 0 ? x : 0; }
    T dx(T x) { return x > 0 ? 1 : 0; }
};

template <Operand T>
struct NegativeOp {
    T operator()(T x) { return -x; }
    T dx(T x) { return -1.0; }
};

template <Operand T>
struct AdditionOp {
    T operator()(T x, T y) { return x + y; }
    T dx(T x, T y) { return 1.0; }
    T dy(T x, T y) { return 1.0; }
};

template <Operand T>
struct ProductOp {
    T operator()(T x, T y) { return x * y; }
    T dx(T x, T y) { return y; }
    T dy(T x, T y) { return x; }
};

template <Operand T>
struct PowOp {
    T operator()(T x, T y) { return pow(x, y); }
    T dx(T x, T y) { return y * pow(x, y - 1.0); }
    T dy(T x, T y) { return log(x) * pow(x, y); }
};

}  // namespace micrograd::operators

#endif  // OPERATORS_HPP