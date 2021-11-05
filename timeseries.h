
#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include "AnomalyDetector.h"
class TimeSeries{
    std::map<std::string, std::vector<float>> table;
public:
    explicit
    TimeSeries(const char* CSVfileName){}

    void readFromFile(std::ifstream file);

    std::vector<std::string> GetFeatures() const;

    std::std::vector<float> GetFeatureVector(const std::string& featureName);
};
#endif