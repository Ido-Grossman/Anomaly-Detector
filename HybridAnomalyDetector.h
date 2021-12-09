

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();

    void learnNormal(const TimeSeries &ts) override;
    bool isAnomaly(float x, float y, correlatedFeatures corelateF) override;
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
