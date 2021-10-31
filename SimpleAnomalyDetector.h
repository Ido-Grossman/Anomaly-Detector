//
// Created by ido on 31/10/2021.
//

#ifndef IDOSANTI_SIMPLEANOMALYDETECTOR_H
#define IDOSANTI_SIMPLEANOMALYDETECTOR_H

struct correlatedFeatures{
    string feature1,feature2; // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
};
class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
public:
    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();
    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    vector<correlatedFeatures> getNormalModel();
};

#endif //IDOSANTI_SIMPLEANOMALYDETECTOR_H
