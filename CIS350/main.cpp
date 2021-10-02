//Program Name: EvaluateInfixOperations
//Programmer Name: Shalom Kebede
//Description: Given a file with infix expressions, the program will create and traverse trees in prefix, infix and postfix order. It will evaluate the expression from the prefix notation and print to the screen the result of the traversal or print error messages as appropriate. 
//Date Created: Sep 18, 2021

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
void constructTree(string, ofstream&);
void printOperations(string, ofstream&);
void prefixEvaluation(string, ofstream&);
string validate(string);
string convertToPostFix(string, ofstream&);
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

void preorder(Node*, ofstream&);
void inorder(Node*, ofstream&);
void postorder(Node*, ofstream&);
int height(Node*);
void printTree(Node*, ofstream&);
void printCurrentLevel(Node*, int, ofstream&);

int main() {

    string postFix;
    string scannedExpression;

    string inputFileName;
    string outputFileName;

    cout << "Welcome! Enter the name of the file you would like to open: ";
    cin >> inputFileName;

    cout << "\nEnter the name of the output file: ";
    cin >> outputFileName;

    //Read from input file & check for file errors
    ifstream inputFile(inputFileName);
    ofstream outputFile(outputFileName);
    
    if (inputFile.fail()) {
        outputFile << "\n ERROR: Cannot read input from file." << endl;
        cout << "\n ERROR: Cannot read input from file." << endl;
        return 0;
    } 
    else if (inputFile.peek() == EOF) {
        outputFile << "\n ERROR: Input file is empty" << endl;
        cout << "\n ERROR: Input file is empty" << endl;

        return 0;
    }

    cout << "\nSuccessfully opened input file: '" << inputFileName << "'" << endl;
    outputFile << "\nSuccessfully opened input file: '" << inputFileName << "'" << endl;

    while (!inputFile.eof())
    {
        string scannedExpression;
        string validationResult;

        getline(inputFile, scannedExpression);
        outputFile << "\n---------------------------------------------------------------------\n\n";
        cout << "\n---------------------------------------------------------------------\n\n" ;
        outputFile << "Input Line: #" << scannedExpression << "#" << endl;
        cout << "Input Line: #" << scannedExpression << "#" << endl;

        validationResult = validate(scannedExpression);

        if (validationResult == "Valid Statement") {
            outputFile << validationResult << endl;
            cout << validationResult << endl;
            postFix = convertToPostFix(scannedExpression, outputFile);
            printOperations(postFix, outputFile);
            constructTree(postFix, outputFile);
        } 
        else {
            outputFile << validationResult;
            cout << validationResult;
        }

    }

    cout << "\nProgram completed.\n";
    outputFile << "\nProgram completed.\n";
    return 0;
}



//Description: Function to validate if scanned expression is valid
//Pre-condition: infix expression (string)
//Post-condition: Returns result of validation
string validate(string expression) {

    string result = "Invalid Statement: \n";
    int countOpening = count(expression.begin(), expression.end(), '(');
    int countClosing = count(expression.begin(), expression.end(), ')');

    vector<int> errors;

    //check for mismatched parenthesis error
    if (countOpening != countClosing) {
        //if not already in error vector, push error 1 (parenthesis mismatched) to errors vector
        if (!(find(errors.begin(), errors.end(), 1) != errors.end())) {
            result += "\tMismatched parenthesis\n";
            errors.push_back(1);
        }
    }

    for (int i = 0; i < expression.length(); i++) {

        //check for invalid operand or operator
        if (isOperand(expression[i]) == false && isOperator(expression[i]) == false && isParenthesis(expression[i]) == false && !isblank(expression[i])) {
            //add error 2 (invalid operand/operator) to results with piece of data causing error
            result += "\tInvalid operator/operand '" + string(1, expression[i]) + "'\n";
            errors.push_back(2);
        }

        //check for spacing error
        //if every even spot is not an empty space, return error 3 (spacing error)
        if (i % 2 == 0) {
            if (!isblank(expression[i])) {
                if (!(find(errors.begin(), errors.end(), 3) != errors.end())) {
                    result += "\tSpacing error\n";
                    errors.push_back(3);
                }
            }
        }

        //check for inaccurate use of parenthesis
        if (isOperator(expression[i])) {
            if ((expression[i + 2] && expression[i + 2] == ')') || (expression[i - 2] && expression[i - 2] == '(')) {
                if (!(find(errors.begin(), errors.end(), 4) != errors.end())) {
                    result += "\tInaccurate use of parenthesis\n";
                    errors.push_back(4);
                }
            }

        }
    }

    //check errors vector for all errors detected
    //if no error detected return 'Valid Statement'
    if (errors.size() == 0) {
        result = "Valid Statement";
    }

    return result;
}

//Description: Function to convert infix expression to postfix
//Pre-condition: infix expression (string), output file 
//Post-condition: Returns post fix expression (string)
string convertToPostFix(string scannedExpression, ofstream& outputFile) {
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

    outputFile << "Postfix Expresssion: " << postFix << endl;
    cout << "Postfix Expresssion: " << postFix << endl;
    return postFix;
}

//Description: Function to check if scanned character is an operand
//Pre-condition: input is char
//Post-condition: returns true if char is operand, false otherwise
bool isOperand(char charRead) {
    if ((charRead >= 'A' && charRead <= 'Z') || (charRead >= '0' && charRead <= '9')) {
        return true;
    }
    else {
        return false;
    }
}

//Description: Function to check if scanned character is an operator
//Pre-condition: input is char
//Post-condition: returns true if char is operand, false otherwise
bool isOperator(char charRead) {
    if ((charRead == '*') || (charRead == '/') || (charRead == '+') || (charRead == '-')) {
        return true;
    }
    else {
        return false;
    }
}

//Description: Function to check if scanned character is an parenthesis
//Pre-condition: input is char
//Post-condition: returns true if char is operand, false otherwise
bool isParenthesis(char charRead) {
    if (charRead == '(' || charRead == ')')
        return true;
    else
        return false;
}

//Description: Function to check the precendence of scanned character
//Pre-condition: input is char
//Post-condition: returns higher precedent if char red is * or /
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

//Description: Function to print operations from postfix expression
//Pre-condition: post fix expression (string), output file
//Post-condition: prints all individual operation in postfix expression & logs to output file
void printOperations(string expression, ofstream& outputFile) {
    stack<string> newStack;
    string operand1;
    string operand2;

    outputFile << "Operations: " << endl;
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
            outputFile << "\t " << operand2 + operand1 + charRead << endl;
            cout << "\t " << operand2 + operand1 + charRead << endl;
        }
    }
}

//Description: Function to construct tree from postfix expression
//Pre-condition: post fix expression (string), output file
//Post-condition: constructs and prints tree, prints prefix, infix and postfix notation, initiates evaluation using prefix
void constructTree(string expression, ofstream& outputFile) {

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

    printTree(s.top(), outputFile);

    outputFile << "\nPrefix: ";
    cout << "\nPrefix: ";
    preorder(s.top(), outputFile);
    
    outputFile << "\nInfix: ";
    cout << "\nInfix: ";
    inorder(s.top(), outputFile);

    outputFile << "\nPostfix: ";
    cout << "\nPostfix: ";
    postorder(s.top(), outputFile);

    prefixEvaluation(prefix, outputFile);

    //reset prefix to empty string
    prefix = "";
}

/* preordertree traversal */
void preorder(Node* ptr, ofstream& outputFile)
{
    //print visit, left, right
    if (ptr) {
        outputFile << ptr->data;
        cout << ptr->data;
        prefix += ptr->data;
        preorder(ptr->left, outputFile);
        preorder(ptr->right, outputFile);
    }

}

/* inordertree traversal */
void inorder(Node* ptr, ofstream& outputFile){
    //print left, visit, right
    if (ptr) {
        inorder(ptr->left, outputFile);
        outputFile << ptr->data;
        cout << ptr->data;
        inorder(ptr->right, outputFile);
    }
}

/* postordertree traversal */
void postorder(Node* ptr, ofstream& outputFile){
    //print left, right, visit
    if (ptr) {
        postorder(ptr->left, outputFile);
        postorder(ptr->right, outputFile);
        outputFile << ptr->data;
        cout << ptr->data;
    }
}


//Description: Print tree in level order
//Pre-condition: root node as input
//Post-condition: prints tree
void printTree(Node* node, ofstream& outputFile)
{
    outputFile << "\nTree: " << endl;
    cout << "\nTree: " << endl;

    int treeHeight = height(node);

    for (int i = 0; i < treeHeight; i++){
        outputFile << "\t";
        cout << "\t";
        printCurrentLevel(node, i, outputFile);
        outputFile << endl;
        cout << endl;
    }
}


//Description: Print node at current level
//Pre-condition: root node as input, level (int), output file
//Post-condition: prints tree
void printCurrentLevel(Node* node, int level, ofstream& outputFile)
{
    if (node != NULL) {
        if (level == 0) {
            outputFile << node->data << " ";
            cout << node->data << " ";
        }
        else if (level > 0)
        {
            //recursively print left and right node, decrementing level to 0
            printCurrentLevel(node->left, level-1, outputFile);
            printCurrentLevel(node->right, level-1, outputFile);
        }
    }
    else {
        return;
    } 

}


//Description: function to get height of tree
//Pre-condition: root node as input
//Post-condition: returns height of tree
int height(Node* node)
{
    if (!node) {
        return 0;
    }

    //get the max height between right and left subtrees, add 1 for current node
    int treeHeight = max(height(node->left), height(node->right)) + 1;

    return treeHeight;
}


//Description: Function to convert char to int
//Pre-condition: char read
//Post-condition: returns converted int
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

//Description: Function to evaluate prefix expression
//Pre-condition: prefix expression (string), output file
//Post-condition: prints evaluation operations and result
void prefixEvaluation(string expression, ofstream& outputFile) {
    stack<string> stringStack;
    stack<double> resultStack;
    string operand1;
    string operand2;

    double oper1;
    double oper2;

    outputFile << "\nEvaluation: " << endl;
    cout << "\nEvaluation: " << endl;

    for (int i = expression.length() -1; i >= 0; i--) {
        char charRead = expression[i];

        if (isOperand(charRead)) {
            stringStack.push(string(1, charRead));

            int digit = convertToDigit(charRead);
            resultStack.push(digit);
        }
        else if (isOperator(charRead)) {
            operand1 = stringStack.top();
            stringStack.pop();

            oper1 = resultStack.top();
            resultStack.pop();

            operand2 = stringStack.top();
            stringStack.pop();

            oper2 = resultStack.top();
            resultStack.pop();

            stringStack.push(charRead + operand1 + operand2);
            outputFile << "\t " << charRead + operand1 + operand2;
            cout << "\t " << charRead + operand1 + operand2;

            //Evaluate expression
            if (charRead == '+') {
                resultStack.push(oper1 + oper2);
            }
            if (charRead == '-') {
                resultStack.push(oper1 - oper2);
            }if (charRead == '*') {
                resultStack.push(oper1 * oper2);
            }
            if (charRead == '/') {
                if (oper2 == 0) {
                    cout << " = ERROR (Division by zero not allowed)" << endl;
                    outputFile << " = ERROR (Division by zero not allowed)" << endl;
                    return;
                }
                resultStack.push(oper1 / oper2);
            }

            outputFile << " = " << resultStack.top() << endl;
            cout << " = " << resultStack.top() << endl;
        }

    }

    //prints result of evaluation
    outputFile << "Final Result: " << resultStack.top() << endl;
    cout << "Final Result: " << resultStack.top() << endl;
}