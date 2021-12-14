

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
struct userAnomaly {
    int startTime;
    int endTime;
};
struct Ts {
    float threshold = 0.9;
    vector<AnomalyReport> reports;
};

// you may edit this class
class Command{
protected:
	DefaultIO* dio;
public:
    const string description;
	Command(DefaultIO* dio, const string desc):dio(dio), description(desc){}
	virtual void execute(struct Ts* ts)=0;
	virtual ~Command(){
        delete dio;
    }
};

class Upload : public Command {
public:
    Upload(DefaultIO* dio):Command(dio, "upload a time series csv file"){}
    void execute(struct Ts* ts) override {
        dio->write("Please upload your local train CSV file.\n");
        dio->readFiles("anomalyTrain.csv");
        dio->write("Upload complete.\n");
        dio->write("Please upload your local test CSV file.\n");
        dio->readFiles("anomalyTest.csv");
        dio->write("Upload complete.\n");
    }
};

class Thresh : public Command {
    float userThreshold = 0;
public:
    Thresh(DefaultIO* dio): Command(dio, "algorithm settings"){}
    void execute(struct Ts* ts) override {
        dio->write("the current correlation threshold is ");
        dio->write(ts->threshold);
        dio->write("\n");
        dio->read(&userThreshold);
        while (userThreshold > 1 && userThreshold < 0) {
            dio->write("please choose a value between 0 and 1");
            dio->read(&userThreshold);
        }
        ts->threshold = userThreshold;
    }
};

class Detect : public Command {
public:
    Detect(DefaultIO* dio):Command(dio, "detect anomalies"){}
    void execute(struct Ts* ts) override {
        TimeSeries train("anomalyTrain.csv");
        TimeSeries test("anomalyTest.csv");
        HybridAnomalyDetector detector;
        detector.setThreshold(ts->threshold);
        detector.learnNormal(train);
        ts->reports = detector.detect(test);
        dio->write("anomaly detection complete");
    }
};

class Results : public Command {
public:
    Results(DefaultIO* dio):Command(dio, "display results"){}
    void execute(struct Ts* ts) override {
        for(const AnomalyReport& report : ts->reports) {
            dio->write((float) report.timeStep);
            dio->write("\t");
            dio->write(report.description);
        }
        dio->write("Done.");
    }
};

class Analyze : public Command {
    vector<userAnomaly> userReports;
public:
    Analyze(DefaultIO* dio): Command(dio, "upload anomalies and analyze results"){}
    void execute(struct Ts* ts) override {
        dio->write("Please upload your local anomalies file.");
        dio->readFiles("userAnomalies.txt");
        std::ifstream file("userAnomalies.txt");
        while (file.good()) {
            std::string line;
            getline(file, line, '\n');
            if (line == "Done.") break;
            ulong where = line.find(',');
            userAnomaly anomaly;
            anomaly.startTime = stoi(line.substr(0, where));
            anomaly.endTime = stoi(line.substr(where));
            userReports.push_back(anomaly);
        }
        file.close();
    }
};

class Fin : public Command {
public:
    Fin(DefaultIO* dio): Command(dio, "exit"){}
    void execute(struct Ts* ts) override {

    }
};

// implement here your command classes



#endif /* COMMANDS_H_ */
