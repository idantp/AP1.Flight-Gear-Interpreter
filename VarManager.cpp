//
// Created by idantp on 12/19/18.
//

#include "VarManager.h"
#include "ShuntingYard.h"

#define COMMA ','
#define ENTER '\n'
#define END_OF_LINE '\0'

VarManager::VarManager() {
    pthread_mutex_init(&mutex, nullptr);
    initializePaths();
    initializeXMLVector();
}

/**
 * Adds a var and it's value to the symbol table.
 * @param name - var name
 * @param value - var value.
 */
void VarManager::addToSymbolTable(string name, double value) {
    if (this->symbolTable.find(name) != symbolTable.end()) {
        throw runtime_error("Variable already exists with that name.");
    }
    this->symbolTable.insert(make_pair(name, value));
}


/**
 * Adds a var and it's value to the binded table.
 * @param name - var name
 * @param value - var value.
 */
void VarManager::addToBindedVars(string name, string path) {
    if (bindedVars.find(name) != bindedVars.end()) {
        throw runtime_error("Variable already exists with that name.");
    }
    this->bindedVars.insert(make_pair(name, path));
}

//return the value of the var by it's name.
double VarManager::getValueByName(string name) {
    if (this->symbolTable.find(name) != symbolTable.end()) {
        return this->symbolTable.find(name)->second;
    }
    throw runtime_error("Could not find in SymbolTable");
}

//return the value of the var by it's path.
double VarManager::getValueByPath(string path) {
    if (this->paths.find(path) != paths.end()) {
        return this->paths.find(path)->second;
    }
    throw runtime_error("Could not find in Paths map");
}

//returns the paths map.
const map<string, double> &VarManager::getPaths() const {
    return paths;
}

//returns the path of the var by it's name.
string VarManager::getPathByName(string name) {
    if (this->bindedVars.find(name) != bindedVars.end()) {
        return this->bindedVars.find(name)->second;
    }
    throw runtime_error("Could not find in BindedVars map.");
}

//set the value of the var by it's name.
void VarManager::setValueByName(string name, double value) {
    if (this->symbolTable.find(name) != symbolTable.end()) {
        this->symbolTable.find(name)->second = value;
    } else {
        throw runtime_error("Could not find in SymbolTable");
    }
}

//set the value of the var by it's path.
void VarManager::setValueByPath(string path, double value) {
    if (this->paths.find(path) != paths.end()) {
        this->paths.find(path)->second = value;
    } else {
        throw runtime_error("Could not find in paths map.");
    }
}

//initialize the paths map.
void VarManager::initializePaths() {
    this->paths.insert(pair<string, double>(
            "/instrumentation/airspeed-indicator/indicated-speed-kt", 0));
    this->paths.insert(pair<string, double>(
            "/instrumentation/altimeter/indicated-altitude-ft", 0));
    this->paths.insert(
            pair<string, double>("/instrumentation/altimeter/pressure-alt-ft",
                                 0));
    this->paths.insert(pair<string, double>(
            "/instrumentation/attitude-indicator/indicated-pitch-deg", 0));
    this->paths.insert(pair<string, double>(
            "/instrumentation/attitude-indicator/indicated-roll-deg", 0));
    this->paths.insert(pair<string, double>(
            "/instrumentation/attitude-indicator/internal-pitch-deg", 0));
    this->paths.insert(pair<string, double>(
            "/instrumentation/attitude-indicator/internal-roll-deg", 0));
    this->paths.insert(pair<string, double>(
            "/instrumentation/encoder/indicated-altitude-ft", 0));
    this->paths.insert(
            pair<string, double>("/instrumentation/encoder/pressure-alt-ft",
                                 0));
    this->paths.insert(
            pair<string, double>("/instrumentation/gps/indicated-altitude-ft",
                                 0));
    this->paths.insert(pair<string, double>(
            "/instrumentation/gps/indicated-ground-speed-kt", 0));
    this->paths.insert(pair<string, double>(
            "/instrumentation/gps/indicated-vertical-speed", 0));
    this->paths.insert(pair<string, double>(
            "/instrumentation/heading-indicator/indicated-heading-deg", 0));
    this->paths.insert(pair<string, double>(
            "/instrumentation/magnetic-compass/indicated-heading-deg", 0));
    this->paths.insert(pair<string, double>(
            "/instrumentation/slip-skid-ball/indicated-slip-skid", 0));
    this->paths.insert(pair<string, double>(
            "/instrumentation/turn-indicator/indicated-turn-rate", 0));
    this->paths.insert(pair<string, double>(
            "/instrumentation/vertical-speed-indicator/indicated-speed-fpm",
            0));
    this->paths.insert(pair<string, double>("/controls/flight/aileron", 0));
    this->paths.insert(pair<string, double>("/controls/flight/elevator", 0));
    this->paths.insert(pair<string, double>("/controls/flight/rudder", 0));
    this->paths.insert(pair<string, double>("/controls/flight/flaps", 0));
    this->paths.insert(
            pair<string, double>("/controls/engines/engine/throttle", 0));
    this->paths.insert(pair<string, double>("/engines/engine/rpm", 0));

}

//set the path of the var by it's name.
void VarManager::setPathByName(string name, string path) {
    if (this->bindedVars.find(name) != bindedVars.end()) {
        this->bindedVars.insert(make_pair(name, path));
    } else {
        throw runtime_error("Could not find in BindedVars map.");
    }

}

//checks if a var exists in the symbol table.
bool VarManager::doesExistInSymbolTable(string name) {
    if (symbolTable.find(name) == symbolTable.end()) {
        return false;
    }
    return true;
}

//checks if a var exists in the binded maps.
bool VarManager::doesExistInBindedVars(string name) {
    if (bindedVars.find(name) == bindedVars.end()) {
        return false;
    }
    return true;
}

//initialize the XML vector according to the xml given.
void VarManager::initializeXMLVector() {
    this->pathsFromXML.emplace_back(
            "/instrumentation/airspeed-indicator/indicated-speed-kt");
    this->pathsFromXML.emplace_back(
            "/instrumentation/altimeter/indicated-altitude-ft");
    this->pathsFromXML.emplace_back(
            "/instrumentation/altimeter/pressure-alt-ft");
    this->pathsFromXML.emplace_back(
            "/instrumentation/attitude-indicator/indicated-pitch-deg");
    this->pathsFromXML.emplace_back(
            "/instrumentation/attitude-indicator/indicated-roll-deg");
    this->pathsFromXML.emplace_back(
            "/instrumentation/attitude-indicator/internal-pitch-deg");
    this->pathsFromXML.emplace_back(
            "/instrumentation/attitude-indicator/internal-roll-deg");
    this->pathsFromXML.emplace_back(
            "/instrumentation/encoder/indicated-altitude-ft");
    this->pathsFromXML.emplace_back("/instrumentation/encoder/pressure-alt-ft");
    this->pathsFromXML.emplace_back(
            "/instrumentation/gps/indicated-altitude-ft");
    this->pathsFromXML.emplace_back(
            "/instrumentation/gps/indicated-ground-speed-kt");
    this->pathsFromXML.emplace_back(
            "/instrumentation/gps/indicated-vertical-speed");
    this->pathsFromXML.emplace_back(
            "/instrumentation/heading-indicator/indicated-heading-deg");
    this->pathsFromXML.emplace_back(
            "/instrumentation/magnetic-compass/indicated-heading-deg");
    this->pathsFromXML.emplace_back(
            "/instrumentation/slip-skid-ball/indicated-slip-skid");
    this->pathsFromXML.emplace_back(
            "/instrumentation/turn-indicator/indicated-turn-rate");
    this->pathsFromXML.emplace_back(
            "/instrumentation/vertical-speed-indicator/indicated-speed-fpm");
    this->pathsFromXML.emplace_back("/controls/flight/aileron");
    this->pathsFromXML.emplace_back("/controls/flight/elevator");
    this->pathsFromXML.emplace_back("/controls/flight/rudder");
    this->pathsFromXML.emplace_back("/controls/flight/flaps");
    this->pathsFromXML.emplace_back("/controls/engines/engine/throttle");
    this->pathsFromXML.emplace_back("/engines/engine/rpm");
}

//updates the XML variables by what the server provided us with.
void VarManager::updateXMLVars(const char *buffer, int size) {
    string temp = "";
    int counter = 0;
    int flag = 0;
    ShuntingYard sh(this);
    for (int i = 0; i < size; i++) {
        if (buffer[i] == END_OF_LINE || i == (size - 1) || buffer[i] == ENTER) {
            flag = 1;
        }
        if (buffer[i] == COMMA || flag == 1) {
            string path = this->pathsFromXML.at(counter);
            Expression *exp = sh.evaluateInfix(temp);
            setValueByPath(path, exp->calculate());
            delete (exp);
            for (auto &it : bindedVars) {
                if (it.second == path) {
                    string varName = it.first;
                    exp = sh.evaluateInfix(temp);
                    setValueByName(varName, exp->calculate());
                    delete (exp);
                }
            }
            if (flag == 1) {
                break;
            }
            counter++;
            temp = "";
            continue;
        }
        temp += buffer[i];
    }
}

//checks if the var exists in the paths map.
bool VarManager::doesExistInPathsMap(string path) {
    if (this->paths.find(path) == paths.end()) {
        return false;
    }
    return true;
}

//get the map of the binded variables.
const map<string, string> &VarManager::getBindedVars() const {
    return bindedVars;
}

