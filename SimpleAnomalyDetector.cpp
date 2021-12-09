// ID: 324603422 (Santiago Szterenberg) && 208985424 (Ido Grossman)
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    _threshold = 0.9;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() = default;

float SimpleAnomalyDetector::calcCfThreshold(Point** points, int &size, Line &linearReg) const {
    float max = 0;
    for (int i = 0; i < size; i++) {
        float distance = std::abs(points[i]->y - linearReg.f(points[i]->x));
        if (distance > max) {
            max = distance;
        }
    }
    return max;
}

void SimpleAnomalyDetector::learnHelper(const TimeSeries& ts, float minThreshold, float maxThreshold) {
    // Getting the features name from the timeseries and the collSize of the features.
    bool isLowerThenMax;
    std::vector<std::string> features = ts.GetFeatures();
    int collSize = ts.GetFeatureVector(features[0]).size();
    ulong size = features.size();
    /*
     * Goes over each one of the features and checks which features correlates the most with it,
     * after it checks for the current feature which feature correlate the most it creates a new correlated features
     * and updates the features and the linear reg of the features.
     */
    for (int i = 0; i < size; i++) {
        float m = 0;
        int c = -1;
        // goes over each of the features that wasn't visited yet and checks which one have the most correlation.
        for (int j = i + 1; j < size; j++) {
            float p = std::abs(pearson(&ts.GetFeatureVector(features[i])[0], &ts.GetFeatureVector(features[j])[0]
                    , collSize));
            if (p > m) {
                m = p;
                c = j;
            }
        }
        // If one of the features had correlation with the current feature it creates them as correlated features.
        if (c != -1 && m > minThreshold) {
            correlatedFeatures coFeatures;
            coFeatures.feature1 = features[i];
            coFeatures.feature2 = features[c];
            coFeatures.corrlation = m;

            std::vector<float> feature1Vec = ts.GetFeatureVector(features[i]);
            std::vector<float> feature2Vec = ts.GetFeatureVector(features[c]);
            std::vector<Point*> points;
            points.reserve(collSize);
            for (int j = 0; j < collSize; j++) {
                points.push_back(new Point(feature1Vec[j], feature2Vec[j]));
            }
            if (coFeatures.threshold < maxThreshold) {
                coFeatures.lowerThenMax = true;
                coFeatures.circle = findMinCircle(&points[0], features.size());
            }
            else {
                coFeatures.lin_reg = linear_reg(&points[0], collSize);
            }
            coFeatures.threshold = calcCfThreshold(&points[0], collSize, coFeatures.lin_reg);
            coFeatures.threshold *= 1.1;
            cf.push_back(coFeatures);
        }
    }
}

/*
 * learns the normal linear reg for each of the correlating features.
 */
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    learnHelper(ts, _threshold, _threshold);
}

/*
 * Detects all the anomalies in the time series
 */
std::vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    // Opening a map of the features and gets the size of cf and the lines size.
    ulong size = cf.size();
    // Creates a vector of anomaly reports.
    std::vector<AnomalyReport> reports;
    for (int i = 0; i < size; i++) {
        // For debugging creates a parameter of the correlated feature and the features name (and Comfortability)
        std::string feature1Name = cf[i].feature1;
        std::string feature2Name = cf[i].feature2;
        std::vector<float> feature1 = ts.GetFeatureVector(cf[i].feature1);
        std::vector<float> feature2 = ts.GetFeatureVector(cf[i].feature2);
        Line lineReg = cf[i].lin_reg;
        float threshold = cf[i].threshold;
        int time = 1;
        std::string description = feature1Name + "-";
        description += feature2Name;
        // Goes over each point in the points vector of the correlated features and checks if it's above or under
        // the _threshold, if it's above it creates it as anomaly and pushes it to the anomaly vector.
        for (int j = 0; j < feature2.size(); j++, time++) {
            float y = feature2[j];
            float x = feature1[j];
            float distanceFromLine = std::abs(y - lineReg.f(x));
            if (distanceFromLine > threshold) {
                AnomalyReport anomalyReport(description, time);
                reports.push_back(anomalyReport);
            }
        }
    }
    return reports;
}
