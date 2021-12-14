

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
    float threshold = 0.9;
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
    Upload(DefaultIO* dio, struct Ts* ts): Command(dio, "upload a time series csv file", ts){}
    void execute() override {
        dio->write("Please upload your local train CSV file.\n");
        dio->readFiles("anomalyTrain.csv");
        dio->write("Upload complete.\n");
        ts->train = new TimeSeries("train.csv");
        dio->write("Please upload your local test CSV file.\n");
        dio->readFiles("anomalyTest.csv");
        dio->write("Upload complete.\n");
        ts->test = new TimeSeries("test.csv");
    }
};

class Thresh : Command {
    float userThreshold;
public:
    Thresh(DefaultIO* dio, struct Ts* ts): Command(dio, "algorithm settings", ts){}
    void execute() override {
        dio->write("the current correlation threshold is ");
        dio->write(ts->threshold);
        dio->write("\n");
        dio->read(&userThreshold);
        while (userThreshold > 1 && userThreshold < 0) {
            dio->write("please choose a value between 0 and 1");

        }
    }
};

class Detect : Command {
public:
    Detect(DefaultIO* dio, struct Ts* ts): Command(dio, "detect anomalies", ts){}
};

class Results : Command {
public:
    Results(DefaultIO* dio, struct Ts* ts): Command(dio, "display results", ts){}
};

class Analyze : Command {
public:
    Analyze(DefaultIO* dio, struct Ts* ts): Command(dio, "upload anomalies and analyze results", ts){}
};

class Fin : Command {
public:
    Fin(DefaultIO* dio, struct Ts* ts): Command(dio, "exit", ts){}
};

// implement here your command classes



#endif /* COMMANDS_H_ */
