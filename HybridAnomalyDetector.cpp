#include "HybridAnomalyDetector.h"
#define HYBRIDTHRESHOLD 0.5

HybridAnomalyDetector::HybridAnomalyDetector() = default;

void HybridAnomalyDetector::learnNormal(const TimeSeries &ts) {
    learnHelper(ts, HYBRIDTHRESHOLD, getThreshold());
}

bool HybridAnomalyDetector::isAnomaly(float x, float y, correlatedFeatures corelateF) {
    float cfX = corelateF.cx;
    float cfY = corelateF.cy;
    return ((!corelateF.lowerThenMax && SimpleAnomalyDetector::isAnomaly(x, y, corelateF)) ||
            (corelateF.lowerThenMax && distance(Point(cfX, cfY), Point(x, y)) > corelateF.threshold));
}

HybridAnomalyDetector::~HybridAnomalyDetector() = default;

