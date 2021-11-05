#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>


int main() {
    std::ifstream file("/home/santiago/Desktop/University/Mitkadem/IdoSanti/test.csv");
    std::map<std::string, std::vector<float>> table;
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
    return 0;
}
