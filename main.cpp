#include <iostream>
#include <stack>

std::string getRPNString(std::string) throw(const char *);

double solveRPNString(std::string) throw(const char *);

bool isOperator(char);

int getPriority(char);

int main() {
    std::string RPNString, inputString;

    std::cout << "Enter the mathematical expression: ";
    getline(std::cin, inputString);

    try {
        RPNString = getRPNString(inputString);
        std::cout << "RPN: " << RPNString << std::endl;
        std::cout << "Result: " << solveRPNString(RPNString) << std::endl;
    } catch (const char *description) {
        std::cout << "Fatal error. Description: " << description << std::endl;
    }

    return 0;
}

std::string getRPNString(std::string input) throw(const char *) {
    std::stack<char> operators;
    std::string finalString;
    int parenthesesCount = 0;

    for (int i = 0; i < input.size(); ++i) {
        char currentChar = input[i];
        //Checking if the character is an operator
        if (isOperator(currentChar)) {
            //Check if there is something in the stack to avoid exception
            if (operators.size()) {
                //If the priority of the current operator is lower, we push operator from the stack's top
                //and put it to the final string
                while (getPriority(currentChar) <= getPriority(operators.top())) {
                    finalString.push_back(operators.top());
                    operators.pop();
                    if (!operators.size()) {
                        break;
                    }
                }
            }
            operators.push(currentChar);
            finalString.push_back(' ');
            continue;
        }
        //If character is not an operator, we check if the this character opens math expression
        if (currentChar == '(') {
            parenthesesCount++;
            operators.push(currentChar);
            continue;
        }
        if (currentChar == ')') {
            //Checking if this character closes expression. If it does, we decrease the number of opening braces.
            parenthesesCount--;
            while (operators.top() != '(' && operators.size()) {
                finalString.push_back(operators.top());
                operators.pop();
                if (!operators.size()) {
                    break;
                }
            }
            if (operators.size()) {
                //Popping '('
                operators.pop();
            }
            continue;
        }
        //If the character is the number, we put it to the final string
        if (currentChar != ' ')
            finalString.push_back(currentChar);
    }

    //Throwing exception if not all expressions were closed
    if (parenthesesCount)
        throw "Unclosed parentheses";

    //Flushing everything from the stack to the string
    while (operators.size()) {
        finalString.push_back(operators.top());
        operators.pop();
    }

    return finalString;
}

double solveRPNString(std::string RPNString) throw(const char *) {
    std::stack<double> PRNStack;
    std::string tempDoubleString;
    for (char temp : RPNString) {
        if ((temp == ' ' || isOperator(temp)) && tempDoubleString.size()) {
            PRNStack.push(atof(tempDoubleString.c_str()));
            tempDoubleString.clear();
        } else {
            //Putting the character to the stack if it is a number
            tempDoubleString.push_back(temp);
        }
        //Performing action with needed number of operand
        //according to the operator type if the character is an operator
        if (isOperator(temp)) {
            double topDouble = PRNStack.top();
            PRNStack.pop();
            double secondDouble = PRNStack.top();
            PRNStack.pop();
            switch (temp) {
                case '+':
                    PRNStack.push(secondDouble + topDouble);
                    break;
                case '-':
                    PRNStack.push(secondDouble - topDouble);
                    break;
                case '*':
                    PRNStack.push(secondDouble * topDouble);
                    break;
                case '/':
                    PRNStack.push(secondDouble / topDouble);
                    break;
                default:
                    throw "Wrong operator passed!";
            }
        }
    }
    return PRNStack.top();
}

bool isOperator(char c) {
    return c == '+' || c == '-' ||
           c == '/' || c == '*' ||
           c == '^';
}

int getPriority(char first) {
    return first == '^' ? 3 :
           first == '*' || first == '/' ? 2 :
           first == '+' || first == '-' ? 1 : 0;
}

