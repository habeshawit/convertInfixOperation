#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <iomanip>  


using namespace std;

bool isOperand(char);
bool isOperator(char);
bool isParenthesis(char);
int precedent(char);
void constructTree(string);
void printOperations(string);
string validate(string);
string convertToPostFix(string);

int space = 7;
int leftSpace = space + 4;
struct Node
{
    char data;
    Node* left, * right;
};

//stack to store pointers
stack<Node*> s;

void preorder(Node*);
void inorder(Node*);
void postorder(Node*);
void levelOrderTraversal(Node*);
int height(Node*);
void printLevelOrder(Node* root);

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
        cout << "\n\nInput Line: #" << scannedExpression << "#" << endl;

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

//Function to print operations
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
 
    //struct Node
    //{
    //    char data;
    //    Node* left, * right;
    //};

    ////stack to store pointers
    //stack<Node*> s;

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
            //cout << node->left->data << endl;
            //cout << node->data << endl;
            //cout << node->right->data << endl;

        }
    }


    printLevelOrder(s.top());

    cout << "\nPrefix: ";
    preorder(s.top());

    cout << "\nInfix: ";
    inorder(s.top());

    cout << "\nPostfix: ";
    postorder(s.top());

    /*while (!s.empty())
    {
        Node *w = s.top();
        cout << "current node " << w->data << endl;

        cout << "print left " << w->left->data << endl;
        cout << "print right " << w->right->data << endl;

        if(w->left->left !=nullptr)
            cout << "print 2nd level left " << w->left->left->data << endl;

        if (w->left->right != nullptr)
            cout << "print 2nd level right " << w->left->right->data << endl;

        if (w->right->left != nullptr)
            cout << "print 3rd level left " << w->right->left->data << endl;

        if (w->right->right != nullptr)
            cout << "print 3rd level right " << w->right->right->data << endl;

        s.pop();
    }*/

}


void preorder(Node* ptr)
{
    if (ptr) {
        cout << ptr->data; // visit
        preorder(ptr->left);// left
        preorder(ptr->right);// right}}
    }
}

void inorder(Node* ptr)/* inordertree traversal */ {
    if (ptr) {
        inorder(ptr->left);// left
        cout << ptr->data;// visit
        inorder(ptr->right);// right
    }
}

void postorder(Node* ptr)/* postordertree traversal */ {
    if (ptr) {
        postorder(ptr->left);// left
        postorder(ptr->right);// right
        cout << ptr->data;// visit
    }
}


//Print Tree in Level Order
void printLevelOrder(Node* root)
{
    cout << "\nPrinting tree top to bottom" << endl;

    int h = height(root);
    int i;

    for (i = 1; i <= h; i++)
    {
        printGivenLevel(root, i);
        cout << endl;
    }
}


/* Print nodes at a given level */
void printGivenLevel(Node* root, int level)
{
    if (root == NULL)
        return;
    if (level == 1) {
        cout << root->data << " ";
    }
    else if (level > 1)
    {
        printGivenLevel(root->left, level - 1);
        printGivenLevel(root->right, level - 1);
    }
}


//Function to get height of node
int height(Node* root)
{
    if (root == NULL)
        return 0;
    return max(height(root->left), height(root->right)) + 1;
}

