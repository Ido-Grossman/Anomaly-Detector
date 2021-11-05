//
// Created by santiago on 05/11/2021.
//

#ifndef IDOSANTI_TIMESERIES_H
#define IDOSANTI_TIMESERIES_H
#include "AnomalyDetector.h"
class TimeSeries{
    std::map<std::string, std::vector<float>> table;

public:
    TimeSeries()= default;
    void readFromFile(std::ifstream file) {
        std::vector<std::string> keys;
        bool isFirstLine = true;

        while(file.good()){
            std::string line;
            getline(file, line, '\n');
            std::istringstream newLine(line);
            std::string word;
            int i = 0;
            while(getline(newLine, word, ',')){
                if (isFirstLine) {
                    std::vector<float> vec;
                    keys.push_back(word);
                    table.insert(std::make_pair(word, vec));
                } else {
                    table[keys[i]].push_back(std::stof(word));
                }
                i++;
            }
            isFirstLine = false;
        }
    }

    std::vector<float> GetFeature(std::string& featureName) const {
        return table.extract(featureName);
    }
};
#endif //IDOSANTI_TIMESERIES_H
