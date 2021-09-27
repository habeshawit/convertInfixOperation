#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>


using namespace std;

void convertExpression(ifstream& inputFile, ofstream& outputFile);
bool isOperand(char);
bool isOperator(char);
bool isParenthesis(char);
//bool isValidStatement(ifstream&);
int precedent(char);
string validate(string);
//string getExpression(char);

string convertToPostFix(string);
void buildTree(string);


int main() {
    string inputFileName = "file.txt";
    string outputFileName = "out.txt";
    string postFix;
    string scannedExpression;

    //cout << "Welcome! Enter the name of the file you would like to open: ";
    //cin >> inputFileName;

    //cout << "\nEnter the name of the output file: ";
    //cin >> outputFileName;

    //Read from input file & check for file errors
    ifstream inputFile(inputFileName);
    ofstream outputFile(outputFileName);

    cout << "Program reading from " << inputFileName << endl;
    outputFile << "Program reading from " << inputFileName << endl;

    while (!inputFile.eof())
    {
        string scannedExpression;
        string validationResult;

        getline(inputFile, scannedExpression);
        cout << "Input Line: #" << scannedExpression << "#" << endl;

        validationResult = validate(scannedExpression);

        //if (validationResult == "Valid Statement") {
        //    postFix = convertToPostFix(scannedExpression);
        //    buildTree(postFix);
        //} 
        //else {
        //    cout << validationResult << endl;
        //}

    }

    return 0;
}

void convertExpression(ifstream& inputFile, ofstream& outputFile) {

}

bool isOperand(char charRead) {
    if ((charRead >= 'A' && charRead <= 'Z') || (charRead >= '0' && charRead <= '9')) {
        return true;
    }
    else {
        return false;
    }
}

bool isOperator(char charRead) {
    if ((charRead == '*') || (charRead == '/') || (charRead == '+') || (charRead == '-')) {
        return true;
    }
    else {
        return false;
    }
}

int precedent(char charRead) {
    if (charRead == '*' || charRead == '/') {
        return 2;
    }
    else if (charRead == '+' || charRead == '-') {
        return 1;
    }
    else {
        return 0;
    }
}

bool isParenthesis(char charRead) {
    if (charRead == '(' || charRead == ')')
        return true;
    else
        return false;
}

//bool isValidStatement(ifstream& inputFile) {
//    char charRead;
//    while (inputFile >> charRead) {
//
//    }
//
//
//}

//string getExpression(char charRead) {
//
//}


string validate(string expression) {

    string result = "Invalid Statement: \n";
    int countOpening = count(expression.begin(), expression.end(), '(');
    int countClosing = count(expression.begin(), expression.end(), ')');
    vector< int > errors;


    //check for mismatched parenthesis error
    if (countOpening != countClosing) {
        if (!(find(errors.begin(), errors.end(), 1) != errors.end())) {
            errors.push_back(1);
        }
        //result += "\tMismatched parenthesis\n";
    }

    for (int i = 0; i < expression.length(); i++) {

        //check for invalid operand or operator
        if (isOperand(expression[i])== false && isOperator(expression[i]) == false && isParenthesis(expression[i]) == false && !isblank(expression[i])) {
            if (!(find(errors.begin(), errors.end(), 2) != errors.end())) {
                errors.push_back(2);
            }

        }

        //check for spacing error
        if (i % 2 == 0) {
            if (!isblank(expression[i]) ){
                if (!(find(errors.begin(), errors.end(), 3) != errors.end())) {
                    errors.push_back(3);
                }
            }
        }
    }

    //check errors vector for all errors detected
    if (errors.size() == 0) {
        result = "Valid Statement";
    }
    else {
        for (int i = 0; i < errors.size(); i++) {
            switch (errors.at(i)) {
            case 1:
                result += "\tMismatched parenthesis\n";
                break;
            case 2:
                result += "\tInvalid operand or operator detected\n";
                break;
            case 3:
                result += "\tSpacing error\n";
                break;
            }
        }

    }

    cout << result;
    return result;

}
