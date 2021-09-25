#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <vector>


using namespace std;

void convertExpression(ifstream& inputFile, ofstream& outputFile);
bool isOperand(char);
bool isOperator(char);
bool isParenthesis(char);
int precedent(char);


int main() {
    string filename;
    string postFix;

    cout << "Welcome! Enter the name of the file you would like to open: ";
    cin >> filename;

    //Read from input file & check for file errors
    ifstream inputFile(filename);

    if (!inputFile) {
        cout << "Could not find/open the input file" << endl;
        return 0;
    } 
    else if (inputFile.peek() == std::ifstream::traits_type::eof()) {
        cout << "File is empty" << endl;
        return 0;
    }

    // initialize stack
    stack <char> myStack;
    
    char charRead;

    while (inputFile >> charRead) {

        //if read character is operand, add to postFix string
        if (isOperand(charRead)) {
            postFix += charRead;
            //cout << charRead;
        }
        //if read character is operator, add to stack
        else if (isOperator(charRead)) {

            //pop all Stack operators if scanned operand has lower precendent than existing precedence operator in stack
            //add popped operator to postFix string
            while (!myStack.empty() && precedent(charRead) < precedent(myStack.top())) {
                postFix += myStack.top();
                myStack.pop();
            }

            //push operand into stack
            myStack.push(charRead);
        }
        else if (isParenthesis(charRead)) {
            if (charRead == '(')
                myStack.push(charRead);
            else {
                while (!myStack.empty()) {
                    cout << myStack.top() << endl;
                    myStack.pop();
                }
            }
        }

    };

    cout << "postFix: " << postFix << endl;

    cout << "stack size: " << myStack.size() << endl;
    
    while (!myStack.empty()) {
        cout << myStack.top() << endl;       
        myStack.pop();
    }


    //
    /*for (int i = 0; i < my_stack.size(); i++) {
        cout << my_stack[i] << endl;
    }*/


    return 0;

    //ofstream outputFile("inputFile.txt", ios::out | ios::trunc);
    //ifstream inputFile("inputFile.txt", ios::in);

    //ifstream inputFile;
    //inputFile.open("inputFile.dat");
    //ofstream outputFile;
    //outputFile.open("outputFile.dat");

    //convertExpression(inputFile, outputFile);

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