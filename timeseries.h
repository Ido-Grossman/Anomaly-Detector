
#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include "AnomalyDetector.h"
class TimeSeries{
    std::map<std::string, std::vector<float>> table;
public:
    explicit
	TimeSeries(const char* CSVfileName);
    void readFromFile(std::ifstream file);
    std::vector<float> getFeatureVector(const std::string& featureName) const;
    std::vector<std::string> getFeatures() const;
};
#endif