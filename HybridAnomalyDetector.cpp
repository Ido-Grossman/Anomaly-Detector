#include "HybridAnomalyDetector.h"
#define MAXTHRESHOLD 0.9

HybridAnomalyDetector::HybridAnomalyDetector() {
    setThreshold(0.5);
}

void HybridAnomalyDetector::learnNormal(const TimeSeries &ts) {
    learnHelper(ts, getThreshold(), MAXTHRESHOLD);
}

HybridAnomalyDetector::~HybridAnomalyDetector() = default;

