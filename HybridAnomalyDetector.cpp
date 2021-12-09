#include "HybridAnomalyDetector.h"
#define MAXTHRESHOLD 0.9

HybridAnomalyDetector::HybridAnomalyDetector() {
    setThreshold(0.5);
}

void HybridAnomalyDetector::learnNormal(const TimeSeries &ts) {
    learnHelper(ts, getThreshold(), MAXTHRESHOLD);
}

bool HybridAnomalyDetector::isAnomaly(float x, float y, correlatedFeatures corelateF) {
    float cfX = corelateF.cx;
    float cfY = corelateF.cy;
    return ((!corelateF.lowerThenMax && SimpleAnomalyDetector::isAnomaly(x, y, corelateF)) ||
            (corelateF.lowerThenMax && distance(Point(cfX, cfY), Point(x, y)) > corelateF.threshold));
}

HybridAnomalyDetector::~HybridAnomalyDetector() = default;

