#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    commands.push_back(new Upload(dio));
    commands.push_back(new Thresh(dio));
    commands.push_back(new Detect(dio));
    commands.push_back(new Results(dio));
    commands.push_back(new Analyze(dio));
    commands.push_back(new Fin(dio));
}

void CLI::start(){
    Ts ts;
    int userInput = 0;
    while (userInput != 5) {
        this->dio->write("Welcome to the Anomaly Detection Server.\n");
        this->dio->write("Please choose an option:\n");
        string toPrint = commands[0]->description;
        for (int index = 1; index < 7; ++index) {
            dio->write(index);
            dio->write(". ");
            dio->write(toPrint + "\n");
            if (index != 6)
                toPrint = commands[index]->description;
        }
        string stringUserInput = dio->read();
        userInput = toIntFromStr(stringUserInput);
        if (userInput <= 6 && userInput >= 0)
            commands[userInput]->execute(&ts);
    }
}

int CLI::toIntFromStr(string numStr) {
    return numStr[0] - '0' - 1;
}

CLI::~CLI() {
    for (auto & command : commands) {
        delete command;
    }
}

