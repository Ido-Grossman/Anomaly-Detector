// ID: 324603422 (Santiago Szterenberg) && 208985424 (Ido Grossman)
#include "timeseries.h"

// constructor for TimeSeries
TimeSeries::TimeSeries(const char *CSVfileName) {
    // opening the file and creating a vector of the keys in the map
    std::ifstream file(CSVfileName);
    std::vector<std::string> keys;
    bool isFirstLine = true;

    while(file.good()){
        std::string line;
        // reading each line separately
        getline(file, line, '\n');
        // entering the line to a stream
        std::istringstream newLine(line);
        std::string word;
        int i = 0;
        // getting each word every time
        while(getline(newLine, word, ',')){
            // if this is the first line then we enter it to the keys vector and creating a new pair in our table
            if (isFirstLine) {
                std::vector<float> vec;
                keys.push_back(word);
                table.insert(std::make_pair(word, vec));
            } else {
                // if this isn't the first line then we enter the number to the correct key
                std::string currentFeature = keys[i];
                table[currentFeature].push_back(std::stof(word));
            }
            i++;
        }
        isFirstLine = false;
    }
    this->keysVector = keys;
    // closing the file
    file.close();
}
std::vector<float> TimeSeries::GetFeatureVector(const std::string &featureName) const {
    return table.find(featureName) -> second;
}

const std::vector<std::string>& TimeSeries::GetFeatures() const {
    return this->keysVector;
}

TimeSeries::~TimeSeries()=default;