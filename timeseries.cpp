#include "timeseries.h"


TimeSeries::TimeSeries(const char *CSVfileName) {
    std::ifstream file(CSVfileName);
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
    file.close();
}
std::vector<float> TimeSeries::getFeatureVector(const std::string &featureName) const {
    return table.find(featureName) -> second;
}

std::vector<std::string> TimeSeries::getFeatures() const {
    std::vector<std::string> keys;
    for (auto it = this->table.begin(); it != this->table.end(); ++it) {
        keys.push_back(it->first);
    }
    return keys;
}