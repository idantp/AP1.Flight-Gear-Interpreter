//
// Created by idantp on 12/17/18.
//
#define SPACE 32
#define OPEN_BRACKET 40
#define CLOSE_BRACKET 41

#include "ShuntingYard.h"
#include <bits/stdc++.h>

using namespace std;

ShuntingYard::ShuntingYard(VarManager *varManager) {
    this->varManager = varManager;
}

/*
 * sets priorities to the operations
 */
int ShuntingYard::operationPriority(char operation) {
    if (operation == '*' || operation == '/') {
        return 2;
    } else if (operation == '+' || operation == '-') {
        return 1;
    } else {
        return 0;
    }
}

/*
 * the function gets 2 Expressions and 1 operation. it calculates the first
 * Expression and the second Expression
 * via the given operation.
 */
Expression *ShuntingYard::applyOperation(Expression *left, Expression *right,
                                         char operation) {
    switch (operation) {
        case '*':
            return new Multiply(left, right);
            break;
        case '/':
            return new Divide(left, right);
            break;
        case '+':
            return new Plus(left, right);
            break;
        case '-':
        default:
            return new Minus(left, right);
            break;
    }
}

/*
 * the function gets a string which describes an expression and calculates
 * it using applyOperation method.
 */
Expression *ShuntingYard::evaluateInfix(string expression) {
    bool minusOp = false;
    //contains all the operation
    stack<char> operations;
    //contains all the values
    stack<Expression *> numbers;
    //running over each char of the given expression
    for (int index = 0; index < expression.length(); index++) {
        if (expression.at(index) == SPACE) {
            continue;
            //if the current char is '(' - push it to operations.
        } else if (expression.at(index) == OPEN_BRACKET) {
            operations.push(expression.at(index));
            //if the current char is a digit - push it to operations.
        } else if (isdigit(expression.at(index))) {
            //will hold the current number which is consists of the current
            // chars.
            double temp = 0;
            double tempDecimal = 0;
            int exponent = 1;
            minusOp = true;
            //turns a string that consists of numbers into an integer.
            while (index < expression.length() &&
                   isdigit(expression.at(index))) {
                temp *= 10;
                temp += (expression.at(index) - '0');
                index++;
            }
            //implements shunting yard on decimal numbers
            if (index < expression.length() && expression.at(index) == '.') {
                index++;
                while (index < expression.length() && isdigit(expression.at
                        (index))) {
                    tempDecimal += ((double) (expression.at(index) - '0') /
                                    pow(10, exponent));
                    exponent++;
                    index++;
                }
                temp += tempDecimal;
            }
            index--;
            //push the number to the numbers' stack
            numbers.push(new Number(temp));
            //if the current char is ')' - push it to the stack, solve what's
            // inside the braces
        } else if (expression.at(index) == CLOSE_BRACKET) {
            //calculate all the operations in the stack operations until
            // we face '('
            while (operations.top() != OPEN_BRACKET && !operations.empty()) {
                Expression *val2 = numbers.top();
                numbers.pop();
                Expression *val = numbers.top();
                numbers.pop();
                char operation = operations.top();
                operations.pop();
                numbers.push(applyOperation(val, val2, operation));
            }
            operations.pop();
            //checks if it is one of the operators - +,-,/,*
        } else if (operationPriority(expression[index]) != 0) {
            //if the current minus is not a binary expression, but unary
            if (!minusOp) {
                Expression *zeroNum = new Number(0);
                numbers.push(zeroNum);
            }
            //while the top operation of the operations stack has same or
            // greater precedence
            //to the current operation, apply that operation.
            while (!operations.empty() && minusOp && (operationPriority
                                                              (expression.at(
                                                                      index)) <=
                                                      operationPriority(
                                                              operations.top()))) {
                Expression *val2 = numbers.top();
                numbers.pop();
                Expression *val = numbers.top();
                numbers.pop();
                char operation = operations.top();
                operations.pop();
                numbers.push(applyOperation(val, val2, operation));
            }
            operations.push(expression.at(index));
            minusOp = false;
        } else {
            string variableName = "";
            minusOp = true;
            while (operationPriority(expression[index]) == 0 &&
                   index < expression.length() &&
                   expression.at(index) != CLOSE_BRACKET) {
                if (expression.at(index) != SPACE) {
                    variableName += expression[index];
                }
                index += 1;
            }
            Expression *variableValue = new Number
                    (this->varManager->getValueByName(variableName));
            numbers.push(variableValue);
            index -= 1;
        }
    }
    //after the whole expression has been parsed, apply the remaining
    // operations with
    // to remaining values
    while (!operations.empty()) {
        Expression *val2 = numbers.top();
        numbers.pop();
        Expression *val = numbers.top();
        numbers.pop();
        char operation = operations.top();
        operations.pop();
        numbers.push(applyOperation(val, val2, operation));
    }
    return numbers.top();
}


