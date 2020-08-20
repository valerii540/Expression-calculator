#include "ExpressionParser.h"
#include <iostream>
#include <vector>
#include <cassert>

int main(int argc, char** argv) {
    cout.precision(21);
    //    if (argc < 2) throw invalid_argument("Expression must be specified as cmd line argument");
    if (argc < 2) {
        while(1) {
            string input;

            cout << "Input: ";
            getline(cin, input);

            ExpressionParser parser(input);
            cout << parser.stringExperssion() << "= " << parser.calculate() << endl << endl;
        }

    } else {
        ExpressionParser parser(argv[1]);

        cout << parser.stringExperssion() << "= " << parser.calculate() << endl;
    }

    return 0;
}
