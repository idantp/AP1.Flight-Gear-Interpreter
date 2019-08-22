//
// Created by idantp on 12/19/18.
//

#include <iostream>
#include "AssignCommand.h"
#include "ShuntingYard.h"

#define NUM_OF_ARG 1
#define SPACE 32
#define ENTER '\r'
#define ENTER2 '\n'

AssignCommand::AssignCommand(VarManager *varManager,
                             OpenClientSocket *openClientSocket)
        : openClientSocket(
        *openClientSocket) {
    this->varManager = varManager;
}

/*
 * implements '=' Command
 */
int AssignCommand::execute(int &index, vector<string> data) {
    ShuntingYard sh(varManager);
    Expression *exp = sh.evaluateInfix(data.at(index + 1));
    double result = exp->calculate();
    delete (exp);
    string varName = data.at(index - 1);
    string writeToServer = "set ";
    //var is binded to a certain path.
    if (varManager->doesExistInBindedVars(varName)) {
        string path = varManager->getPathByName(varName);
        writeToServer += path;
        writeToServer += SPACE;
        writeToServer += to_string(result);
        writeToServer += ENTER;
        writeToServer += ENTER2;
        this->openClientSocket.writeToSimulator(writeToServer.c_str());
        //variable is an XML
        if (varManager->doesExistInPathsMap(path)) {
            //set the value in paths map.
            varManager->setValueByPath(path, result);
        }
        //go over all binded vars and update whoever is binded to the same path.
        for (auto &it : varManager->getBindedVars()) {
            if (it.second == path) {
                varManager->setValueByName(it.first, result);
            }
        }

    } else {
        this->varManager->setValueByName(varName, result);
    }
    return NUM_OF_ARG;
}
