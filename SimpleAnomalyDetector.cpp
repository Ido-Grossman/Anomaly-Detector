
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
	_threshold = 0.9;

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() = default;


void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    // Gets the vector of features and calculates the size of features and the column size of features
    const std::vector<std::string>& features = ts.GetFeatures();
    int featuresAmount = features.size();
    int  collSize = ts.GetFeatureVector(features[0]).size();
    // Creates a table to save the variables in it and saves the vectors in the table.
    float table[featuresAmount][collSize];
    for (int i = 0; i < featuresAmount; i++) {
        std::vector<float> coll = ts.GetFeatureVector(features[i]);
        for (int j = 0; j < collSize; j++) {
            table[i][j] = coll[j];
        }
    }
    // does the algorithm suggested by the exercise and creates correlated feature struct for each correlated features.
    for (int i = 0; i < featuresAmount; i++) {
        float m = 0;
        int c = -1;
        for (int j = i + 1; j < featuresAmount; j++) {
            float p = std::abs(pearson(table[i], table[j], collSize));
            if (p > m) {
                m = p;
                c = j;
            }
        }
        // If the features are correlated it creates a variable for them.
        if (c != -1 && m > _threshold) {
            Point* points[collSize];
            for (int j = 0; j < collSize; j++) {
                points[j] = new Point(table[i][j], table[c][j]);
            }
            buildCf(features[i], features[c], points, collSize, m);
        }
    }
}



std::vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
	std::vector<std::string> features = ts.GetFeatures();
    int cfSize = cf.size();
    int collSize = ts.GetFeatureVector(features[0]).size();
    std::vector<AnomalyReport> reports;
    for (int i = 0; i < cfSize; i++) {
        std::vector<float> features1 = ts.GetFeatureVector(cf[i].feature1);
        std::vector<float> features2 = ts.GetFeatureVector(cf[i].feature2);
        int timeStep = 1;
        for (int j = 0; j < collSize; j++, timeStep++) {
            float x = std::abs(features2[j] - cf[i].lin_reg.f(features1[j]));
            if (x > cf[i].threshold) {
                AnomalyReport report(cf[i].feature1 + "-" + cf[i].feature2, timeStep);
                reports.push_back(report);
            }
        }
    }
    return reports;
}

/*
 * Creates the correlated features based on the given parameters.
 */
void SimpleAnomalyDetector::buildCf(std::string feature1, std::string feature2, Point** points, int &featureSize, float& m) {
    correlatedFeatures coFeatures;
    coFeatures.feature1 = feature1;
    coFeatures.feature2 = feature2;
    coFeatures.corrlation = m;
    coFeatures.lin_reg = linear_reg(points, featureSize);
    coFeatures.threshold = calcCfThreshold(points, featureSize, coFeatures.lin_reg) * 1.1;
    cf.push_back(coFeatures);
}

/*
 * calculates what is the point that is the farthest from the line and what is her distance from the line.
 */
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

