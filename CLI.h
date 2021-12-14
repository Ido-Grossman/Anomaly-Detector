

#ifndef CLI_H_
#define CLI_H_

#include <cstring>
#include "commands.h"

using namespace std;

class CLI {
	DefaultIO* dio;
    Command* commands;
	TimeSeries* trainTs;
    TimeSeries* testTs;
public:
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();
};

#endif /* CLI_H_ */
