

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <cstring>
#include <cmath>

struct correlatedFeatures{
	std::string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
	std::vector<correlatedFeatures> cf;
public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();

	virtual void learnNormal(const TimeSeries& ts);
	virtual std::vector<AnomalyReport> detect(const TimeSeries& ts);

	std::vector<correlatedFeatures> getNormalModel(){
		return cf;
	}

};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
