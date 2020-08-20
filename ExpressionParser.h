#ifndef EXPRESSIONPARSER_H
#define EXPRESSIONPARSER_H

#include "Node.h"
#include <algorithm>
#include <regex>

enum OperatorPriority {
    pTimes = 1, pDivide = 1, pPlus = 2, pMinus = 2
};

class ExpressionParser {
    vector<string> vec_;
    Node           root_;

    void build(Node& root, vector<string> vec) {
        //        cout << "build vec: ";
        //        for(auto i: vec)
        //            cout << i << ' ';
        //        cout << endl;

        struct Point {
            string tag; int pos; int pr;
        } point;

        if (vec.size() == 1) {
            root.leftChild_ = NULL;
            root.rightChild_ = NULL;
            root.literal_ = vec[0];

            return;
        }

        point.pos = -1;
        point.tag = "#";
        point.pr  = 0;

        for (unsigned int i = 0; i < vec.size(); i++) {
            if ((vec[i] == "*" || vec[i] == "/") && point.pr <= pTimes) {
                point.pos = i;
                point.pr = pTimes;
                point.tag = vec[i];
            }
            else if ((vec[i] == "-" || vec[i] == "+") && point.pr <= pPlus) {
                point.pos = i;
                point.pr = pPlus;
                point.tag = vec[i];
            }
        }
        if (point.tag == "#") cout << "Error while building" << '\n';

        root.literal_ = point.tag;

        if (vec.begin() != vec.begin() + point.pos) {
            vector<string>* leftVec = new vector<string>(vec.begin(), vec.begin() + point.pos);
            root.leftChild_ = new Node(point.tag);
            build(*root.leftChild_, *leftVec);
            delete leftVec;
        } else cout << "Left child is empty" << endl;

        if (vec.begin() != vec.begin() + point.pos + 1) {
            vector<string>* rightVec = new vector<string>(vec.begin() + point.pos + 1, vec.end());
            root.rightChild_ = new Node(point.tag);
            build(*root.rightChild_, *rightVec);
            delete rightVec;
        } else cout << "Right child is empty" << endl;
    }

    void traverseInorder(Node& node, string& acc) {
        if (node.leftChild_ != NULL)
            traverseInorder(*node.leftChild_, acc);

        acc += node.literal_ + " ";

        if (node.rightChild_ != NULL)
            traverseInorder(*node.rightChild_, acc);
    }

    vector<string> resolveParenthesis(const vector<string>& tokens) {
        int starting_par = tokens.size() - distance(tokens.rbegin(), find(tokens.rbegin(), tokens.rend(), "("));
        int closing_par  = distance(tokens.begin(), find(tokens.begin() + starting_par, tokens.end(), ")"));

        //        cout << starting_par << ' ' << closing_par << endl;

        if (starting_par == 0) {
            cout << "After parenthesis resolving: ";
            for (auto t: tokens) cout << t << ' ';
            cout << endl;

            return tokens;
        }

        //        cout << "tokens: ";
        //        for(auto i = starting_par; i != closing_par; i++)
        //            cout << tokens[i] << ' ';
        //        cout << endl;

        vector<string> inner(tokens.begin() + starting_par, tokens.begin() + closing_par);

        cout << "inner expression: ";
        for(auto i: inner)
            cout << i << ' ';
        cout << endl;

        resolveUnaryTokens(inner);

        ExpressionParser inner_parser(inner);
        double inner_result = inner_parser.calculate();

        vector<string> refined_tokens;
        refined_tokens.insert(refined_tokens.end(), tokens.begin(), tokens.begin() + starting_par - 1);
        refined_tokens.push_back(Node::to_string(inner_result));
        refined_tokens.insert(refined_tokens.end(), tokens.begin() + closing_par + 1, tokens.end());

        cout << "refined: ";
        for(auto x: refined_tokens)
            cout << x << ' ';
        cout << endl;

        return resolveParenthesis(refined_tokens);
    }

    void resolveUnaryTokens(vector<string>& tokens) {
        if (tokens.size() >= 2 && tokens[0] == "-" && !Node::isOperator(tokens[1])) {
            tokens[1] = Node::to_string(atof(tokens[1].c_str()) * -1);
            tokens.erase(tokens.begin());
        }

        for(auto i = 0; i < tokens.size() - 1; i++)
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

        cout << "After unary resolving: ";
        for (auto t: tokens) cout << t << ' ';
        cout << endl;
    }

public:
    ExpressionParser(const string expression) {
        root_ = Node("#");

        const regex r("\\d+\\.?\\d*|\\-|\\+|\\*|\\/|\\(|\\)");

        sregex_iterator iter(expression.begin(), expression.end(), r);
        sregex_iterator end;

        vector<string> tokens;
        for(; iter != end; iter++)
            for(unsigned i = 0; i < iter->size(); ++i) {
                // std::cout << (*iter)[i] << ' ';
                tokens.push_back(((*iter)[i]).str());
            }
        // cout << endl;

        vec_ = resolveParenthesis(tokens);
        resolveUnaryTokens(vec_);

        build(root_, vec_);
    }

    ExpressionParser(vector<string>& vec) {
        root_ = Node("#");
        vec_  = vector<string>(vec);

        build(root_, vec_);
    }

    string stringExperssion() {
        string exprStr = "";
        traverseInorder(root_, exprStr);
        return exprStr;
    }

    double calculate() { return root_.calculate(); }

};

#endif //EXPRESSIONPARSER_H
