#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>

using namespace std;

class Node {
public:
    Node*     leftChild_;
    Node*     rightChild_;
    string    literal_;

    Node():
        leftChild_(nullptr),
        rightChild_(nullptr),
        literal_("#")
    {}

    Node(string literal):
        leftChild_(nullptr),
        rightChild_(nullptr),
        literal_(literal)
    {}

    ~Node() {
        if (leftChild_)  delete leftChild_;
        if (rightChild_) delete rightChild_;
    }

    static bool isOperator(string token) {
        return token == "+" || token == "-" || token == "*" || token == "/";
    }

    double calculate() {
        if (literal_ == "-")
            return leftChild_->calculate() - rightChild_->calculate();
        else if (literal_ == "+")
            return leftChild_->calculate() + rightChild_->calculate();
        else if (literal_ == "*")
            return leftChild_->calculate() * rightChild_->calculate();
        else if (literal_ == "/")
            return leftChild_->calculate() / rightChild_->calculate();
        else
            return atof(literal_.c_str());
    }

    static string to_string(const double a_value, const int n = 21) {
        ostringstream out;
        out.precision(n);
        out << fixed << a_value;
        return out.str();
    }
};

#endif // NODE_H
