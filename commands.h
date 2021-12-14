

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <utility>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}
	void readFiles(string fileName){
        ofstream fileOut(fileName);
        string bits;
        while ((bits = read()) != "done"){
            fileOut << bits << endl;
        }
        fileOut.close();
    }
};

// you may add here helper classes

struct Ts {
    TimeSeries* train;
    TimeSeries* test;
};

// you may edit this class
class Command{
protected:
	DefaultIO* dio;
    struct Ts* ts;
public:
    const string description;
	Command(DefaultIO* dio, const string desc, struct Ts* ts):dio(dio), description(desc), ts(ts){}
	virtual void execute()=0;
	virtual ~Command(){
        delete dio;
    }
};

class Upload : Command {
public:
    void execute() override {
        dio->write("Please upload your local train CSV file.");
        dio->readFiles("train.csv");
        dio->write("Upload complete.");
        dio->write("Please upload your local train CSV file.");
        dio->readFiles("test.csv");
        dio->write("Upload complete.");
    }
};

// implement here your command classes



#endif /* COMMANDS_H_ */
