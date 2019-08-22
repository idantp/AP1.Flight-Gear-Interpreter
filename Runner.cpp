//
// Created by idantp on 12/24/18.
//

#include "Runner.h"
#include "Parser.h"
#include <vector>
#include "CommandExpression.h"
#include "Command.h"
#include "OpenDataServerCommand.h"
#include "LoopCommand.h"
#include "ConditionParser.h"
#include "IfCommand.h"
#include "VarManager.h"
#include "BindCommand.h"
#include "OpenClientSocket.h"

#define OPEN_SERVER "openDataServer"
#define IF_CONDITION "if"
#define WHILE_LOOP "while"
#define CONNECT "connect"
#define VAR "var"
#define PRINT "print"
#define SLEEP "sleep"
#define ASSIGN "="
#define BIND "=bind"

Runner::Runner(const char *fileName) {
    this->varManager = new VarManager();
    varManager->incCount();
    Lexer lexer;
    openClientSocket = new OpenClientSocket();
    this->lexeredFile = lexer.lexerFromFile(fileName);
    this->commandsMap = commandsGenerator(lexeredFile);
    this->index = 0;

}

/**
 * Generates the maps for the commands. Initializes the values and creates
 * them dynamically to later be used by every execute function.
 *
 * @param lexStrings - vector of lexered strings.
 * @return - the map after it was generated.
 */
map<string, Expression *> Runner::commandsGenerator(vector<string> lexStrings) {
    map<string, Expression *> commandsExMap;

    commandsExMap.insert(make_pair(BIND, new CommandExpression(
            new BindCommand(varManager), lexStrings, index)));
    commandsExMap.insert(
            make_pair(OPEN_SERVER,
                      new CommandExpression(
                              new OpenDataServerCommand(varManager), lexStrings,
                              index)));
    commandsExMap.insert(
            make_pair(IF_CONDITION, new CommandExpression(
                    new IfCommand(varManager, &commandsMap), lexStrings,
                    index)));
    commandsExMap.insert(
            make_pair(WHILE_LOOP, new CommandExpression(
                    new LoopCommand(varManager, &commandsMap), lexStrings,
                    index)));
    commandsExMap.insert(make_pair(CONNECT, new CommandExpression(
            new ConnectCommand(openClientSocket, varManager),
            lexStrings, index)));
    commandsExMap.insert(
            make_pair(VAR,
                      new CommandExpression(new CreateVarCommand(varManager),
                                            lexStrings, index)));
    commandsExMap.insert(
            make_pair(PRINT, new CommandExpression(new PrintCommand(varManager),
                                                   lexStrings, index)));
    commandsExMap.insert(
            make_pair(SLEEP, new CommandExpression(new SleepCommand(varManager),
                                                   lexStrings, index)));
    commandsExMap.insert(
            make_pair(ASSIGN, new CommandExpression(
                    new AssignCommand(varManager, openClientSocket), lexStrings,
                    index)));
    return commandsExMap;
}

/**
 * Runs the program.
 */
void Runner::run() {
    Parser parser(this->lexeredFile, this->varManager, &commandsMap, index);
    parser.parseLexer();
}

/**
 * Destructor.
 */
Runner::~Runner() {
    for (map<string, Expression *>::iterator it = this->commandsMap.begin();
         it != this->commandsMap.end(); it++) {
        delete (*it).second;
    }
    varManager->signalFinished();
    if (varManager->decCount() == 0) {
        delete varManager;
    }

    delete openClientSocket;
}
