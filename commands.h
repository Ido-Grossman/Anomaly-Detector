

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
        delete ts;
    }
};

class Upload : Command {
public:
    Upload(DefaultIO* dio, const string desc, struct Ts* ts): Command(dio, desc, ts){}
    void execute() override {
        dio->write("Please upload your local train CSV file.");
        dio->readFiles("train.csv");
        dio->write("Upload complete.");
        ts->train = new TimeSeries("train.csv");
        dio->write("Please upload your local train CSV file.");
        dio->readFiles("test.csv");
        dio->write("Upload complete.");
        ts->test = new TimeSeries("test.csv");
    }
};

class Thresh : Command {
public:
    Thresh(DefaultIO* dio, const string desc, struct Ts* ts): Command(dio, desc, ts){}
};

class Detect : Command {
public:
    Detect(DefaultIO* dio, const string desc, struct Ts* ts): Command(dio, desc, ts){}
};

class Results : Command {
public:
    Results(DefaultIO* dio, const string desc, struct Ts* ts): Command(dio, desc, ts){}
};

class Analyze : Command {
public:
    Analyze(DefaultIO* dio, const string desc, struct Ts* ts): Command(dio, desc, ts){}
};

class Fin : Command {
public:
    Fin(DefaultIO* dio, const string desc, struct Ts* ts): Command(dio, desc, ts){}
};

// implement here your command classes



#endif /* COMMANDS_H_ */
