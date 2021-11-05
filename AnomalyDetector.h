#ifndef IDOSANTI_ANOMALYDETECTOR_H
#define IDOSANTI_ANOMALYDETECTOR_H
#include <vector>
#include <map>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include "timeseries.h"

class AnomalyReport{
public:
    const std::string description;
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
