

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <iostream>
#include <cstring>

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
//    This method reads from a file
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
    /*
    * First requests the user to upload is train CSV, and then upload his test CSV, and it reads the files and
    * stores the train in anomalyTrain.csv and the test in anomalyTest.csv.
    */
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
    /*
     * Prints the user the current threshold and gives him the option to change the threshold to his desired
     * threshold.
     */
    void execute(struct Ts* ts) override {
        dio->write("The current correlation threshold is ");
        dio->write(ts->threshold);
        dio->write("\n");
        dio->write("Type a new threshold\n");
        dio->read(&userThreshold);
        // Makes sure the user inputted threshold is valid and between 0 and 1.
        while (userThreshold > 1 && userThreshold < 0) {
            dio->write("please choose a value between 0 and 1.\n");
            dio->read(&userThreshold);
        }
        ts->threshold = userThreshold;
    }
};

class Detect : public Command {
public:
    Detect(DefaultIO* dio):Command(dio, "detect anomalies"){}
    /*
     * By the train and test files given in 1 the hybrid anomaly detector detects all the anomalies in the csv file
     * the user uploaded.
     */
    void execute(struct Ts* ts) override {
        TimeSeries train("anomalyTrain.csv");
        TimeSeries test("anomalyTest.csv");
        HybridAnomalyDetector detector;
        detector.setThreshold(ts->threshold);
        detector.learnNormal(train);
        ts->reports = detector.detect(test);
        ts->size = test.GetFeatureVector(test.GetFeatures()[0]).size();
        dio->write("anomaly detection complete.\n");
    }
};

class Results : public Command {
public:
    Results(DefaultIO* dio):Command(dio, "display results"){}
    /*
     * Displays the detected anomalies in the user train and test files.
     */
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
    /*
     * Given the original reports of the anomaly detector it merges all the reports that their description matches and
     * their time step is +1 after the previous time step.
     */
    void mergeReports(struct Ts* ts, vector<Anomaly>& originReports) {
        ulong size = ts->reports.size();
        for (int i = 0; i < size; i++) {
            Anomaly anomaly;
            anomaly.startTime = ts->reports[i].timeStep;
            anomaly.endTime = ts->reports[i].timeStep;
            anomaly.description = ts->reports[i].description;
            /*
             * Goes through the time steps from the detector and checks if they are right one after another and if
             * they are it merges them.
             */
            for (int j = i + 1; j < size; j++) {
                if (ts->reports[j].timeStep == anomaly.endTime + 1 &&
                    ts->reports[j].description == anomaly.description) {
                    anomaly.endTime = ts->reports[j].timeStep;
                } else {
                    originReports.push_back(anomaly);
                    i = j - 1;
                    break;
                }
                if (j == size - 1) {
                    originReports.push_back(anomaly);
                }
                i = j;
            }
        }
    }
public:
    Analyze(DefaultIO* dio): Command(dio, "upload anomalies and analyze results"){}
    /*
     * Takes a reports file from the user and checks which of the reports the detector detected are true and which
     * are false.
     */
    void execute(struct Ts* ts) override {
        vector<Anomaly> userReports;
        vector<Anomaly> originReports;
        dio->write("Please upload your local anomalies file.\n");
        // Merges all the original reports.
        mergeReports(ts, originReports);
        string line = "";
        // Reads the user file and pushes all his reports into a vector.
        while ((line = dio->read()) != "done") {
            ulong where = line.find(',');
            Anomaly anomaly;
            anomaly.startTime = stoi(line.substr(0, where));
            anomaly.endTime = stoi(line.substr(where + 1));
            userReports.push_back(anomaly);
        }
        dio->write("Upload complete.\n");
        ulong p = userReports.size();
        ulong n = ts->size;
        ulong fp = 0;
        ulong tp = 0;
        bool foundOne;
        for (int i = 0; i < p; i++, foundOne = false) {
            n -= (userReports[i].endTime - userReports[i].startTime) + 1;
        }
        /*
         * For each original reports the detector found it checks if there is a report the user uploaded that
         * is in the same time with the original report, if there is it marks the report as positive, else it marks
         * it as false.
         */
        for (Anomaly& originReport : originReports) {
            for (Anomaly& userReport : userReports) {
                if ((originReport.startTime >= userReport.startTime &&
                    originReport.startTime <= userReport.endTime) ||
                    (originReport.endTime >= userReport.startTime &&
                    originReport.endTime <= userReport.endTime) ||
                    (originReport.startTime <= userReport.endTime &&
                    originReport.endTime >= userReport.endTime)) {
                    tp++;
                    foundOne = true;
                    break;
                }
            }
            if (!foundOne)
                fp++;
            foundOne = false;
        }
        // Calculates the true positive rate and false positive rate up to 3 digits after the dot.
        float trueRate = floorf(((float) tp / (float) p) * 1000) / 1000;
        float falseRate = floorf(((float) fp / (float) n) * 1000) / 1000;
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




#endif /* COMMANDS_H_ */
