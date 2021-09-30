#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

bool isOperand(char);
bool isOperator(char);
bool isParenthesis(char);
int precedent(char);
void constructTree(string);
void printOperations(string);

void prefixEvaluation(string);

string validate(string);
string convertToPostFix(string);
string prefix;
int convertToDigit(char);

//struct to create node for binary tree
struct Node
{
    char data;
    Node* left, * right;
};

//stack to store pointers to node
stack<Node*> s;

void preorder(Node*);
void inorder(Node*);
void postorder(Node*);
int height(Node*);
void printTree(Node*);
void printCurrentLevel(Node*, int);
void evaluateExpression(string);


int main() {
    string inputFileName;
    string outputFileName;
    string postFix;
    string scannedExpression;

    cout << "Welcome! Enter the name of the file you would like to open: ";
    cin >> inputFileName;

    cout << "\nEnter the name of the output file: ";
    cin >> outputFileName;

    //Read from input file & check for file errors
    ifstream inputFile(inputFileName);
    ofstream outputFile(outputFileName);
    
    if (inputFile.fail()) {
        cout << "\nERROR: Cannot read input from file." << endl;
        return 0;
    } 
    else if (inputFile.peek() == EOF) {
        cout << "\n Input file is empty" << endl;
        return 0;
    }

    cout << "Program reading from " << inputFileName << endl;
    outputFile << "Program reading from " << inputFileName << endl;

    while (!inputFile.eof())
    {
        string scannedExpression;
        string validationResult;

        getline(inputFile, scannedExpression);
        cout << "\n---------------------------------------------------------------------\n\n" ;
        cout << "Input Line: #" << scannedExpression << "#" << endl;

        validationResult = validate(scannedExpression);

        if (validationResult == "Valid Statement") {
            cout << validationResult << endl;
            postFix = convertToPostFix(scannedExpression);
            printOperations(postFix);
            constructTree(postFix);
        } 
        else {
            cout << validationResult;
        }

    }

    return 0;
}

//Function to convert infix expression to postfix
string convertToPostFix(string scannedExpression) {
    stack <char> myStack;
    string postFix;

    for (int i = 0; i < scannedExpression.length(); i++) {
        char charRead = scannedExpression[i];

        //if read character is operand, add to postFix string
        if (isOperand(charRead)) {
            postFix += charRead;
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
        else if(isParenthesis(charRead)) {
            if (charRead == '(')
                myStack.push(charRead);
            else {
                while (!myStack.empty() && myStack.top() != '(') {
                    postFix += myStack.top();
                    myStack.pop();
                }

                if (!myStack.empty())
                    myStack.pop();
            }

        }

    };

    //Pop remaining items in the stack
    while (!myStack.empty()) {
        postFix += myStack.top();
        myStack.pop();
    }

    cout << "Postfix Expresssion: " << postFix << endl;
    return postFix;
}

//Function to check if scanned character is an operand
bool isOperand(char charRead) {
    if ((charRead >= 'A' && charRead <= 'Z') || (charRead >= '0' && charRead <= '9')) {
        return true;
    }
    else {
        return false;
    }
}

//Function to check if scanned character is an operator
bool isOperator(char charRead) {
    if ((charRead == '*') || (charRead == '/') || (charRead == '+') || (charRead == '-')) {
        return true;
    }
    else {
        return false;
    }
}

//Function to check if scanned character is a parenthesis
bool isParenthesis(char charRead) {
    if (charRead == '(' || charRead == ')')
        return true;
    else
        return false;
}

//Function to check the precendence of scanned character
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


//Function to validate if scanned expression is valid
string validate(string expression) {

    string result = "Invalid Statement: \n";
    int countOpening = count(expression.begin(), expression.end(), '(');
    int countClosing = count(expression.begin(), expression.end(), ')');

    vector<int> errors;

    //check for mismatched parenthesis error
    if (countOpening != countClosing) {
        if (!(find(errors.begin(), errors.end(), 1) != errors.end())) {
            errors.push_back(1);
        }
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
                result += "\tInvalid operand or operator\n";
                break;
            case 3:
                result += "\tSpacing error\n";
                break;
            }
        }

    }

    return result;
}

//Function to print operations from postfix expression
void printOperations(string expression) {
    stack<string> newStack;
    string operand1;
    string operand2;

    cout << "Operations: " << endl;

    for (int i = 0; i < expression.length(); i++) {
        char charRead = expression[i];

        if (isOperand(charRead)) {
            newStack.push(string(1, charRead));
        }
        else if (isOperator(charRead)) {
            operand1 = newStack.top();
            newStack.pop();

            operand2 = newStack.top();
            newStack.pop();

            newStack.push(operand2 + operand1 + charRead);
            cout << "\t " << operand2 + operand1 + charRead << endl;
        }
    }
}

void constructTree(string expression) {

    for (int i = 0; i < expression.length(); i++) { 
        char charRead = expression[i];

        //if operand, turn into leaf node and push to stack
        if (isOperand(charRead)) {
            Node* node = new Node();
            node ->data = charRead;
            node->left = node->right = nullptr;
            s.push(node);
        } 

        //if operator
        else {
            //create node with operator as node
            Node *node = new Node();
            node->data = charRead;

            //Pop item from stack and set as 1st child
            Node *child1 = s.top();
            s.pop();

            //Pop item from stack and set as 2nd child
            Node *child2 = s.top();
            s.pop();

            //set right and left children
            node->right = child1;
            node->left = child2;

            //add subtree to stack
            s.push(node);
        }
    }

    printTree(s.top());

    cout << "\nPrefix: ";
    preorder(s.top());
    
    cout << "\nInfix: ";
    inorder(s.top());

    cout << "\nPostfix: ";
    postorder(s.top());

    prefixEvaluation(prefix);

    //reset prefix to empty string
    prefix = "";
}

/* preordertree traversal */
void preorder(Node* ptr)
{
    if (ptr) {
        cout << ptr->data;
        prefix += ptr->data;
        preorder(ptr->left);
        preorder(ptr->right);
    }

}

/* inordertree traversal */
void inorder(Node* ptr){
    if (ptr) {
        inorder(ptr->left);
        cout << ptr->data;
        inorder(ptr->right);
    }
}

/* postordertree traversal */
void postorder(Node* ptr){
    if (ptr) {
        postorder(ptr->left);
        postorder(ptr->right);
        cout << ptr->data;
    }
}


//Print Tree in Level Order
void printTree(Node* node)
{
    cout << "\nTree: " << endl;

    int treeHeight = height(node);

    for (int i = 0; i < treeHeight; i++){
        cout << "\t";
        printCurrentLevel(node, i);
        cout << endl;
    }
}


//Print node at current level
void printCurrentLevel(Node* node, int level)
{
    if (node != NULL) {
        if (level == 0) {
            cout << node->data << " ";
        }
        else if (level > 0)
        {
            //recursively print left and right node, decrementing level to 0
            printCurrentLevel(node->left, level-1);
            printCurrentLevel(node->right, level-1);
        }
    }
    else {
        return;
    } 

}

//Function to get height of tree
int height(Node* node)
{
    if (!node) {
        return 0;
    }

    //get the max height between right and left subtrees, add 1 for current node
    int treeHeight = max(height(node->left), height(node->right)) + 1;

    return treeHeight;
}



void evaluateExpression(string expression) {
    cout << "\nEvaluations: " << endl;

    stack<double> Stack;
    string finalExpression = "";
    string currentOperation = "";
    vector< string > arr;
    vector< double > summation;

    for (int i = expression.size() - 1; i >= 0; i--) {
        char charRead = expression[i]; 
       

        // Push operand to Stack
        if (isOperand(charRead)) {
            int digit = convertToDigit(charRead);
            Stack.push(digit);
            currentOperation += charRead;
        }  
        else {
            //if operator read, pop two elements from the stack
            finalExpression = charRead;

            double oper1 = Stack.top();
            cout << "o " << oper1 << endl;
            Stack.pop();

            double oper2 = Stack.top();
            cout << "o " << oper2 << endl;

            Stack.pop();

            for (int i = currentOperation.length()-1; i >= 0; i--) {
                finalExpression += currentOperation[i];
            }
            currentOperation = "";


            arr.push_back(finalExpression);

            //Evaluate expression
            if (charRead == '+') {
                Stack.push(oper1 + oper2);
            }
            if (charRead == '-') {
                Stack.push(oper1 - oper2);
            }if (charRead == '*') {
                Stack.push(oper1 * oper2);
            }
            if (charRead == '/') {
                Stack.push(oper1 / oper2);
            }

            summation.push_back(Stack.top());


        }
    }

    for (int i = 0; i < arr.size() - 1; i++) {
        cout << "\t" << arr[i] << " = " << summation[i] << endl;
    }
    cout << "\t" << prefix << " = " << Stack.top() << endl;

    //prints result of evaluation
    cout << "\n\tFinal Result: " << Stack.top();
}


int convertToDigit(char charRead) {
    //Convert numbers in expression to int by subtracting '0'
    if (isdigit(charRead)) {
        return charRead - '0';
    }
    //Convert alphabets in expression to int by subtracting 'A' and adding 1
    else {
        return charRead - 'A' + 1;
    }
}


void prefixEvaluation(string expression) {
    stack<string> stringStack;
    stack<double> sumStack;
    string operand1;
    string operand2;

    double oper1;
    double oper2;

    cout << "\nEvaluation: " << endl;

    for (int i = expression.length() -1; i >= 0; i--) {
        char charRead = expression[i];

        if (isOperand(charRead)) {
            stringStack.push(string(1, charRead));

            int digit = convertToDigit(charRead);
            sumStack.push(digit);
        }
        else if (isOperator(charRead)) {
            operand1 = stringStack.top();
            stringStack.pop();

            oper1 = sumStack.top();
            sumStack.pop();

            operand2 = stringStack.top();
            stringStack.pop();

            oper2 = sumStack.top();
            sumStack.pop();

            stringStack.push(charRead + operand1 + operand2);
            cout << "\t " << charRead + operand1 + operand2;

            //Evaluate expression
            if (charRead == '+') {
                sumStack.push(oper1 + oper2);
            }
            if (charRead == '-') {
                sumStack.push(oper1 - oper2);
            }if (charRead == '*') {
                sumStack.push(oper1 * oper2);
            }
            if (charRead == '/') {
                sumStack.push(oper1 / oper2);
            }

            cout << " = " << sumStack.top() << endl;
        }

    }

    //prints result of evaluation
    cout << "Final Result: " << sumStack.top() << endl;
}