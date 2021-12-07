
#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

class TimeSeries{
    // table we create from the csv file
    std::map<std::string, std::vector<float>> table;
    // the keys vector of the map
    std::vector<std::string> keysVector;
public:
    explicit
    TimeSeries(const char* CSVfileName);

    ~TimeSeries() = default;

    // this function returns the keys vector
    const std::vector<std::string>& GetFeatures() const;

    // this function given a key as an argument, returns the vector of the feature
    std::vector<float> GetFeatureVector(const std::string& featureName) const;

};
#endif