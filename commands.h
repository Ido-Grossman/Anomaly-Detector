

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
	virtual ~DefaultIO()= default;
	void readFiles(string fileName){
        ofstream fileOut(fileName);
        string bits ="";
        // in mainTrain we finish with "done"
        while ((bits = read()) != "done"){
            fileOut << bits << endl;
        }
        fileOut.close();
    }
};

// you may add here helper classes
struct Anomaly {
    long startTime;
    long endTime;
    string description;
};
struct Ts {
    float threshold = 0.9;
    vector<AnomalyReport> reports;
    ulong size;
};

// you may edit this class
class Command{
protected:
	DefaultIO* dio;
public:
    const string description;
	Command(DefaultIO* dio, const string desc):dio(dio), description(desc){}
	virtual void execute(struct Ts* ts)=0;
	virtual ~Command()= default;
};

class Upload : public Command {
public:
    Upload(DefaultIO* dio): Command(dio, "upload a time series csv file"){}
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
        ts->size = test.GetFeatureVector(test.GetFeatures()[0]).size();
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
            dio->write(report.description + "\n");
        }
        dio->write("Done.\n");
    }
};

class Analyze : public Command {
private:
    vector<Anomaly> userReports;
    vector<Anomaly> originReports;
    void mergeReports(struct Ts* ts) {
        ulong size = ts->reports.size();
        for (int i = 0; i < size; i++) {
            Anomaly anomaly;
            anomaly.startTime = ts->reports[i].timeStep;
            anomaly.endTime = ts->reports[i].timeStep;
            anomaly.description = ts->reports[i].description;
            for (int j = i; j < size; j++) {
                if (ts->reports[j].timeStep == anomaly.endTime + 1 &&
                    ts->reports[j].description == anomaly.description) {
                    anomaly.endTime = ts->reports[j].timeStep;
                } else {
                    originReports.push_back(anomaly);
                    i = j;
                    break;
                }
                i = j;
            }
        }
    }
public:
    Analyze(DefaultIO* dio): Command(dio, "upload anomalies and analyze results"){}
    void execute(struct Ts* ts) override {
        dio->write("Please upload your local anomalies file.");
        dio->readFiles("userAnomalies.txt");
        dio->write("Upload complete.");
        mergeReports(ts);
        std::ifstream file("userAnomalies.txt");
        while (file.good()) {
            std::string line;
            getline(file, line, '\n');
            if (line == "Done.") break;
            ulong where = line.find(',');
            Anomaly anomaly;
            anomaly.startTime = stoi(line.substr(0, where));
            anomaly.endTime = stoi(line.substr(where));
            userReports.push_back(anomaly);
        }
        file.close();
        ulong p = userReports.size();
        ulong n = ts->size;
        ulong fp = 0;
        ulong tp = 0;
        for (int i = 0, j = 0; i < p; i++) {
            n -= (userReports[i].endTime - userReports[i].startTime) + 1;
            for (; j < p; j++) {
                if (userReports[j].startTime > userReports[i].endTime) {
                    fp++;
                    break;
                } else if ((userReports[j].startTime >= userReports[i].startTime &&
                            userReports[j].startTime < userReports[i].endTime) ||
                            userReports[j].endTime > userReports[i].startTime &&
                            userReports[j].endTime <= userReports[i].endTime) {
                    tp++;
                }
            }
        }
        float trueRate = roundf(((float) tp / (float) p) * 100) / 100;
        float falseRate = roundf(((float) fp / (float) n) * 100) / 100;
        dio->write("True Positive Rate: ");
        dio->write(trueRate);
        dio->write("\n");
        dio->write("False Positive Rate: ");
        dio->write(falseRate);
        dio->write("\n");
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
