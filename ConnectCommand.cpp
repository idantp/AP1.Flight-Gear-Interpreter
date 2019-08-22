//
// Created by idantp on 12/19/18.
//

#include "ConnectCommand.h"
#include "ShuntingYard.h"
#include <pthread.h>
#include <thread>
#include <iostream>

#define NUM_OF_ARGS 2

struct MyParameters {
    string ip;
    double portNum;
    int socketfd;
    OpenClientSocket *openClientSocket;

    MyParameters(OpenClientSocket *openClientSocket) : openClientSocket(
            openClientSocket) {

    }
};

ConnectCommand::ConnectCommand(OpenClientSocket *openClientSocket,
                               VarManager *varManager) :
        openClientSocket(openClientSocket) {
    this->varManager = varManager;
}

/**
 * executes the connect command.
 * @param index - where the command is in the vector.
 * @param data - vector of code.
 * @return - num of arguments.
 */
int ConnectCommand::execute(int &index, vector<string> data) {
    ShuntingYard shuntingYard(varManager);
    Expression *exp = shuntingYard.evaluateInfix(data.at(index + 2));
    string ip = data.at(index + 1);
    double port = exp->calculate();
    delete exp;
    struct MyParameters *myParameters = new MyParameters(
            this->openClientSocket);
    myParameters->portNum = port;
    myParameters->ip = ip;
    myParameters->socketfd = myParameters->openClientSocket->openSocket(
            myParameters->ip, myParameters->portNum);
    this->varManager->addSockfd(myParameters->socketfd);
    delete myParameters;
    return NUM_OF_ARGS;
}
