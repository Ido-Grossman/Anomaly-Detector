
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() = default;

SimpleAnomalyDetector::~SimpleAnomalyDetector() = default;

/*
 * learns the normal linear reg for each of the correlating features.
 */
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    // Getting the features name from the timeseries and the size of the features.
    std::vector<std::string> features = ts.GetFeatures();
    int size = ts.GetFeatureVector(features[0]).size();
    /*
     * Goes over each and every one of the features and checks which features correlates the most with it,
     * after it checks for the current feature which feature correlate the most it creates a new correlated features
     * and updates the features and the linear reg of the features.
     */
    for (int i = 0; i < features.size(); i++) {
        float m = 0;
        int c = -1;
        // goes over each of the features that wasn't visited yet and checks which one have the most correlation.
        for (int j = i + 1; j < features.size(); j++) {
            float p = std::abs(pearson(&ts.GetFeatureVector(features[i])[0], &ts.GetFeatureVector(features[i])[0]
                    , size));
            if (p > m) {
                m = p;
                c= j;
            }
        }
        // If one of the features had correlation with the current feature it creates them as correlated features.
        if (c != -1) {
            correlatedFeatures coFeatures;
            coFeatures.feature1 = features[i];
            coFeatures.feature2 = features[c];
            coFeatures.corrlation = m;
            std::vector<float> feature1Vec = ts.GetFeatureVector(features[i]);
            std::vector<float> feature2Vec = ts.GetFeatureVector(features[c]);
            std::vector<Point*> points;
            for (int j = 0; j < size; j++) {
                points.insert(points.cend(), new Point(feature1Vec[j], feature2Vec[j]));
            }
            coFeatures.lin_reg = linear_reg(&points[0], size);
        }
    }
}

std::vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
	std::vector<std::string> features = ts.GetFeatures();
    std::map<std::string, std::vector<Point>> points;
    for (int i = 0; i < cf.size(); i++) {
        std::vector<float> feature1 = ts.GetFeatureVector(cf[i].feature1);
        std::vector<float> feature2 = ts.GetFeatureVector(cf[i].feature2);
        std::vector<Point> correlated;
        for (int j = 0; j < feature1.size(); j++) {
            Point p(feature1[j], feature2[j]);
            correlated.push_back(p);
        }
        points.insert(std::make_pair(cf[i].feature1, correlated));
    }

}

