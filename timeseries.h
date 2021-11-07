
#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include "AnomalyDetector.h"
class TimeSeries{
    std::map<std::string, std::vector<float>> table;
public:
    explicit
    TimeSeries(const char* CSVfileName);
    std::vector<std::string> GetFeatures() const;
    std::vector<float> GetFeatureVector(const std::string& featureName) const;
};
#endif