#ifndef EXPRESSIONPARSER_H
#define EXPRESSIONPARSER_H

#include "Node.h"
#include <algorithm>
#include <regex>

enum OperationPriority {
    pTimes = 1, pDivide = 1, pPlus = 2, pMinus = 2
};

class ExpressionParser {
    vector<string> vec_;
    Node           root_;

    void build(Node& root, vector<string> vec) {
        if (vec.size() == 1) {
            root.literal_    = vec[0];

            return;
        }

        int position                 = -1;
        string tag                   = "#";
        unsigned short int priority  = 0;

        for (unsigned long i = 0; i < vec.size(); i++) {
            if ((vec[i] == "*" || vec[i] == "/") && priority <= pTimes) {
                position = i;
                priority = pTimes;
                tag = vec[i];
            }
            else if ((vec[i] == "-" || vec[i] == "+") && priority <= pPlus) {
                position = i;
                priority = pPlus;
                tag = vec[i];
            }
        }
        if (tag == "#") throw runtime_error("Error while building");

        root.literal_ = tag;

        if (vec.begin() != vec.begin() + position) {
            vector<string>* leftVec = new vector<string>(vec.begin(), vec.begin() + position);
            root.leftChild_ = new Node(tag);
            build(*root.leftChild_, *leftVec);
            delete leftVec;
        }

        if (vec.begin() != vec.begin() + position + 1) {
            vector<string>* rightVec = new vector<string>(vec.begin() + position + 1, vec.end());
            root.rightChild_ = new Node(tag);
            build(*root.rightChild_, *rightVec);
            delete rightVec;
        }
    }

    vector<string> resolveParenthesis(const vector<string>& tokens) {
        int starting_par = tokens.size() - distance(tokens.rbegin(), find(tokens.rbegin(), tokens.rend(), "("));
        int closing_par  = distance(tokens.begin(), find(tokens.begin() + starting_par, tokens.end(), ")"));

        if (starting_par == 0) return tokens;

        vector<string> inner(tokens.begin() + starting_par, tokens.begin() + closing_par);

        resolveUnaryTokens(inner);

        ExpressionParser inner_parser(inner);
        double inner_result = inner_parser.calculate();

        vector<string> refined_tokens;
        refined_tokens.insert(refined_tokens.end(), tokens.begin(), tokens.begin() + starting_par - 1);
        refined_tokens.push_back(Node::to_string(inner_result));
        refined_tokens.insert(refined_tokens.end(), tokens.begin() + closing_par + 1, tokens.end());

        return resolveParenthesis(refined_tokens);
    }

    void resolveUnaryTokens(vector<string>& tokens) {
        if (tokens.size() >= 2 && tokens[0] == "-" && !Node::isOperator(tokens[1])) {
            tokens[1] = Node::to_string(atof(tokens[1].c_str()) * -1);
            tokens.erase(tokens.begin());
        }

        for(unsigned long i = 0; i < tokens.size() - 1; i++)
            if (Node::isOperator(tokens[i]) && Node::isOperator(tokens[i+1])) {
                if (tokens[i] == "+" && tokens[i+1] == "-")
                    tokens.erase(tokens.begin() + i);
                else if(tokens[i] == "-" && tokens[i+1] == "+")
                    tokens.erase(tokens.begin() + i + 1);
                else if(tokens[i] == "-" && tokens[i+1] == "-") {
                    tokens[i+1] = "+";
                    tokens.erase(tokens.begin() + i);
                }
                else if(tokens[i+1] == "-") {
                    tokens[i + 2] = Node::to_string(atof(tokens[i + 2].c_str()) * -1);
                    tokens.erase(tokens.begin() + i + 1);
                }
            }
    }

public:
    ExpressionParser(const string expression) {
        root_ = Node("#");

        const regex r("\\d+\\.?\\d*|\\-|\\+|\\*|\\/|\\(|\\)");

        sregex_iterator iter(expression.begin(), expression.end(), r);
        sregex_iterator end;

        vector<string> tokens;
        for(; iter != end; iter++)
            for(unsigned i = 0; i < iter->size(); ++i)
                tokens.push_back(((*iter)[i]).str());

        vec_ = resolveParenthesis(tokens);
        resolveUnaryTokens(vec_);

        build(root_, vec_);
    }

    ExpressionParser(vector<string>& vec) {
        root_ = Node("#");
        vec_  = vector<string>(vec);

        build(root_, vec_);
    }

    double calculate() { return root_.calculate(); }
};

#endif //EXPRESSIONPARSER_H
