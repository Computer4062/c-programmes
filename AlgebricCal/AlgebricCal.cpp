#include <iostream>
#include <string>
#include <fstream>

using namespace std;

char ops[4] = { '+', '-', '*', '/' };

void introText() {
    cout << "============================================================= \n";
    cout << "\t\t\tAlgebraicCal \n";
    cout << "============================================================= \n";
    cout << "Algebraic Cal is a calculator for algebraic equations \n";
    cout << "Here are some examples: \n";
    cout << "\t231+x=234\n";
    cout << "\tx+231=234\n";
    cout << "\t231+234=x\n";
    cout << "type exit to leave \n";
}

void showIntroText() {
    ifstream openedCheck;
    openedCheck.open("openedCheck.dat");

    string line;
    getline(openedCheck, line);
    openedCheck.close();

    if (line == "1"){
        introText();
        ofstream openedCheck;
        openedCheck.open("openedCheck.dat");
        openedCheck << "0";
        openedCheck.close();
    }
}

int main() {
    showIntroText();
    string calculation;

    while(true) {
        string calculation, op, num1, num2, num3;
        double x;
        int opPlace = -1, signPlace, iCount = -1;
        bool doOpCheck = false, doSignCheck = false, count = false;

        cout << ">>"; cin >> calculation;

        if (calculation == "exit") break;

        for (int i = 0; i < calculation.length(); i++) {

            if (doOpCheck == false) {
                for (int j = 0; j < 4; j++) {
                    if (calculation[i] == ops[j]) {
                        op = ops[j];
                        doOpCheck = true;
                        opPlace = i;
                        count = true;
                        break;
                    }
                }
            }

            if (count == true) iCount += 1;

            if (doSignCheck == false) {
                for (int j = 0; j < 4; j++) {
                    if (calculation[i] == '=') {
                        doSignCheck = true;
                        signPlace = i;
                        count = false;
                        break;
                    }
                }
            }
        }

        num1 = calculation.substr(0, opPlace);
        num2 = calculation.substr(opPlace + 1, iCount - 1);
        num3 = calculation.substr(signPlace + 1, -1);

        cout << op << endl;

        if (num1 == "x") {
            num2 = op + num2;
            x = stod(num3) - stod(num2);
            cout << "x = " << x << endl;
        }
        else if (num2 == "x" && op != "-") {
            x = stod(num3) - stod(num1);
            cout << "x = " << x << endl;
        }
        else if (num2 == "x" && op == "-") {
            x = stod(num3) - stod(num1);
            cout << "x = -" << x << endl;
        }
        else if (num3 == "x") {
            x = stod(num1) + stod(num2);
            cout << "x = " << x << endl;
        }
    }

    return 0;
}
