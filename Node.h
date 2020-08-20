#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <cmath>

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
        return token.find_first_of("+-*/") != string::npos;
    }

    double calculate() {
        if (literal_ == "-")
//            return (leftChild_ && !isOperator(leftChild_->literal_) ? leftChild_->calculate() - rightChild_->calculate() : - rightChild_->calculate());
            return leftChild_->calculate() - rightChild_->calculate();
        else if (literal_ == "+")
            return leftChild_->calculate() + rightChild_->calculate();
        else if (literal_ == "*")
            return leftChild_->calculate() * rightChild_->calculate();
        else if (literal_ == "/")
            return leftChild_->calculate() / rightChild_->calculate();
        else if (literal_ == "^")
            return pow(leftChild_->calculate(), rightChild_->calculate());
        else if (literal_ == "√")
            return pow(rightChild_->calculate(), leftChild_->calculate());
        else if (literal_ == "#")
            throw runtime_error("Calculating empty node!");
        else
            return atof(literal_.c_str());
    }
};

#endif // NODE_H
