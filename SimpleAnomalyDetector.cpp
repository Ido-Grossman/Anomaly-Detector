
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() = default;

SimpleAnomalyDetector::~SimpleAnomalyDetector() = default;


void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){

}

std::vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
	std::vector<std::string> features = ts.getFeatures();
    std::map<correlatedFeatures, std::vector<Point>> table;
    for (int i = 0; i < cf.size(); i++) {
        std::vector<float> feature1 = ts.getFeatureVector(cf[i].feature1);
        std::vector<float> feature2 = ts.getFeatureVector(cf[i].feature2);
        std::vector<Point> correlated;
        for (int j = 0; j < feature1.size(); j++) {
            Point p(feature1[j], feature2[j]);
            correlated.push_back(p);
        }
        table.insert(std::make_pair(cf[i], correlated));
    }
}

