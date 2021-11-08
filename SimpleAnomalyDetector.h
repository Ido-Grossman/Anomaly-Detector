

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
	std::string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
	std::vector<correlatedFeatures> cf;
    float _threshold;
public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();

	virtual void learnNormal(const TimeSeries& ts);
	virtual std::vector<AnomalyReport> detect(const TimeSeries& ts);

	std::vector<correlatedFeatures> getNormalModel(){
		return cf;
	}

private:
    void buildCf(std::string feature1, std::string feature2, Point** points, int &featureSize, float& m);
    float calcCfThreshold(Point** points, int &size, Line &linearReg) const;
};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
