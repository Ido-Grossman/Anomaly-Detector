#ifndef IDOSANTI_ANOMALYDETECTOR_H
#define IDOSANTI_ANOMALYDETECTOR_H
#include <vector>
#include <map>
#include <cstring>
#include <iostream>
#include <fstream>

class TimeSeries{
    std::map<std::string, std::vector<float>> table;

public:
    TimeSeries()= default;
    void readFromFile(std::ifstream file) {
        std::vector<std::string> keys;
        bool isFirstLine = true;

        while(file.good()){
            std::string line;
            getline(file, line, '\n');
            std::istringstream newLine(line);
            std::string word;
            int i = 0;
            while(getline(newLine, word, ',')){
                if (isFirstLine) {
                    std::vector<float> vec;
                    keys.push_back(word);
                    table.insert(std::make_pair(word, vec));
                } else {
                    table[keys[i]].push_back(std::stof(word));
                }
                i++;
            }
            isFirstLine = false;
        }
    }

    std::vector<float> GetFeature(std::string& featureName) const {
        return table.extract(featureName);
    }
};
class AnomalyReport{
public:
    const string description;
    const long timeStep;
    AnomalyReport(string description, long timeStep) :
            description(description),timeStep(timeStep){}
};
class TimeSeriesAnomalyDetector {
public:
    virtual void learnNormal(const TimeSeries& ts)=0;
    virtual vector<AnomalyReport> detect(const TimeSeries& ts)=0;
    virtual ~TimeSeriesAnomalyDetector(){}
};

#endif //IDOSANTI_ANOMALYDETECTOR_H
