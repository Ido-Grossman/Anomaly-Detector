
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() = default;

SimpleAnomalyDetector::~SimpleAnomalyDetector() = default;


void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    std::vector<std::string> features = ts.getFeatures();
    int size = ts.getFeatureVector(features[0]).size();
    for (int i = 0; i < features.size(); i++) {
        float m = 0;
        int c = -1;
        for (int j = i + 1; j < features.size(); j++) {
            float p = std::abs(pearson(&ts.getFeatureVector(features[i])[0], &ts.getFeatureVector(features[i])[0]
                    , size));
            if (p > m) {
                m = p;
                c= j;
            }
        }
        if (c != -1) {
            correlatedFeatures coFeatures;
            coFeatures.feature1 = features[i];
            coFeatures.feature2 = features[c];
            coFeatures.corrlation = m;
            std::vector<float> feature1Vec = ts.getFeatureVector(features[i]);
            std::vector<float> feature2Vec = ts.getFeatureVector(features[c]);
            std::vector<Point*> points;
            for (int j = 0; j < size; j++) {
                points.insert(points.cend(), new Point(feature1Vec[j], feature2Vec[j]));
            }
            coFeatures.lin_reg = linear_reg(&points[0], size);
        }
    }
}

std::vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
	// TODO Auto-generated destructor stub
}

