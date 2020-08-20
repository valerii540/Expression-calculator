#include "ExpressionParser.h"
#include <iostream>
#include <vector>
#include <cassert>

int main(int argc, char** argv) {
    cout.precision(21);
    if (argc < 2) {
        while(1) {
            string input;

            cout << "Input: ";
            getline(cin, input);

            ExpressionParser parser(input);
            cout << input << "= " << parser.calculate() << endl << endl;
        }

    } else {
        ExpressionParser parser(argv[1]);

        cout << argv[1] << " = " << parser.calculate() << endl;
    }

    return 0;
}
