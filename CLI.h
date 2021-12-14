

#ifndef CLI_H_
#define CLI_H_

#include <cstring>
#include "commands.h"

using namespace std;

class CLI {
    vector<Command*> commands;
	DefaultIO* dio;
public:
	CLI(DefaultIO* dio);
	void start();
    int toIntFromStr(string numStr);
	virtual ~CLI();
};

#endif /* CLI_H_ */
