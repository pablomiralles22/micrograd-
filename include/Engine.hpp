#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <Operators.hpp>

// Classes for operation tree

namespace micrograd::engine {
template <micrograd::operators::Operand T>
struct Value {
    T val;

    Value(T _val) : val(_val) {}

    void update_grad(T delta){};

    void reset_grad(){};

    void backward_prop(){};
};

template <micrograd::operators::Operand T>
using Constant = Value<T>;

template <micrograd::operators::Operand T>
struct Variable : Value<T> {
    T grad;

    Variable(T _val) : Value<T>(_val), grad(0) {}

    void update_grad(T delta) { grad += delta; }

    void reset_grad() { grad = 0; }
};

template <micrograd::operators::Operand T, micrograd::operators::UnaryOp<T> Op>
struct UnaryOpVariable : Variable<T> {
    Value<T> x;
    Op op;

    UnaryOpVariable(Value<T> _x) : Variable<T>(op(_x.val)), x(_x) {}

    void backward_prop() { x.update_grad(op.dx(x.val) * Variable<T>::grad); }
};

template <micrograd::operators::Operand T, micrograd::operators::BinaryOp<T> Op>
struct BinaryOpVariable : Variable<T> {
    Value<T> x, y;
    Op op;

    BinaryOpVariable(Value<T> _x, Value<T> _y)
        : Variable<T>(op(_x.val, _y.val)), x(_x), y(_y){};

    void backward_prop() {
        x.update_grad(op.dx(x.val, y.val) * Variable<T>::grad);
        y.update_grad(op.dy(x.val, y.val) * Variable<T>::grad);
    }
};

template <micrograd::operators::Operand T>
using ProductVariable = BinaryOpVariable<T, micrograd::operators::ProductOp<T>>;

template <micrograd::operators::Operand T>
using AdditionVariable =
    BinaryOpVariable<T, micrograd::operators::AdditionOp<T>>;

template <micrograd::operators::Operand T>
using ExponentialVariable =
    UnaryOpVariable<T, micrograd::operators::ExponentialOp<T>>;

// OPERATIONS

template <micrograd::operators::Operand T>
Value<T> exp(Value<T> x) {
    return ExponentialVariable<T>(x);
}

template <micrograd::operators::Operand T>
Value<T> operator*(Value<T> x, Value<T> y) {
    return ProductVariable<T>(x, y);
}

template <micrograd::operators::Operand T>
Value<T> operator+(Value<T> x, Value<T> y) {
    return AdditionVariable<T>(x, y);
}

}  // namespace micrograd::engine
// Some utils

template <micrograd::operators::Operand T>
std::ostream &operator<<(std::ostream &os,
                         micrograd::engine::Value<T> const &value) {
    return os << value.val;
}

#endif  // ENGINE_HPP