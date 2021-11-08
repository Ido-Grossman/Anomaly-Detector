

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include "minCircle.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
	std::string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
    Circle circle;
	float threshold;
    bool lowerThenMax = false;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
	std::vector<correlatedFeatures> cf;
    float _threshold;
public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();

    void learnHelper(const TimeSeries& ts, float minThreshold, float maxThreshold);
	virtual void learnNormal(const TimeSeries& ts);
	virtual std::vector<AnomalyReport> detect(const TimeSeries& ts);

    virtual bool isAnomaly(float x, float y, correlatedFeatures cf);
	std::vector<correlatedFeatures> getNormalModel(){
		return cf;
	}
protected:
    std::vector<correlatedFeatures> GetCF() {
        return cf;
    }
    void setThreshold(float threshold) {
        _threshold = threshold;
    }
    float getThreshold() {
        return _threshold;
    }
private:
    void buildCf(std::string feature1, std::string feature2, Point** points, int &featureSize, float& m);
    float calcCfThreshold(Point** points, int &size, Line &linearReg) const;
};





#endif /* SIMPLEANOMALYDETECTOR_H_ */
